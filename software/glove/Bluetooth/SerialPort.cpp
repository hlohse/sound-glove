#include "Bluetooth\SerialPort.h"
#include "Bluetooth\Stack.h"
#include "Bluetooth\SPP.h"
#include "Command.h"
#include "Error.h"
#include "SPPAPI.h"
#include <stdlib.h>
#include <string.h>
using namespace Bluetooth;

SerialPort::ScheduledWrite::ScheduledWrite()
:	command_type(Command::type_invalid),
	serialization()
{
}

SerialPort::ScheduledWrite::ScheduledWrite(uint8_t command_type, const Serialization& serialization)
:	command_type(command_type),
	serialization(serialization)
{
}

bool SerialPort::ScheduledWrite::operator==(const SerialPort::ScheduledWrite& other) const
{
	return command_type == other.command_type;
}

SerialPort& SerialPort::instance()
{
    static SerialPort instance;
    return instance;
}

SerialPort::SerialPort()
:   scheduled_writes_(),
	read_buffer_(),
    read_buffer_length_(0),
	write_offset_(0)
{
}

bool SerialPort::isOpen()
{
	SPP& spp = SPP::instance();
	return spp.isOpen() && spp.isConnected();
}

uint8_t SerialPort::peek(const int index)
{
	if (index >= 0 && index < read_buffer_length_) {
		return read_buffer_[index];
	}
	else {
        Error::instance().set(Error::TYPE_BT_SERIAL_PORT_PEEK_EXCEED, index);
		return 0;
	}
}

void SerialPort::read(uint8_t* destination, const int length)
{
    if (length <= 0) {
        Error::instance().set(Error::TYPE_BT_SERIAL_PORT_READ_NULL, length);
        return;
    }
    if (length > read_buffer_length_) {
        Error::instance().set(Error::TYPE_BT_SERIAL_PORT_READ_EXCEED,
                              length - read_buffer_length_);
        return;
    }
    memcpy(destination, read_buffer_, length);
    read_buffer_length_ -= length;
    memmove(read_buffer_, read_buffer_ + length, read_buffer_length_);
}

void SerialPort::receiveData()
{
	if (read_buffer_length_ < BLUETOOTH_SERIAL_PORT_READ_BUFFER_SIZE) {
		const int status = SPP_Data_Read(Stack::instance().id(),
										 SPP::instance().id(),
										 BLUETOOTH_SERIAL_PORT_READ_BUFFER_SIZE - read_buffer_length_,
										 read_buffer_ + read_buffer_length_);
		if (status >= 0) {
			read_buffer_length_ += status;
		}
		else {
			Error::instance().set(Error::TYPE_BT_SERIAL_PORT_DATA_READ, status);
		}
	}
}

void SerialPort::write(uint8_t* source, const int length)
{
    Error::instance().set(Error::TYPE_BT_SERIAL_PORT_WRITE_RAW, Error::no_info);
}

void SerialPort::write(const uint8_t command_type, const Serialization& serialization)
{
    if (serialization.size() <= 0) {
        Error::instance().set(Error::TYPE_BT_SERIAL_PORT_WRITE_NULL, serialization.size());
    }
    else {
    	tryScheduleWrite(command_type, serialization);
    	issueWrite();
    }
}

void SerialPort::tryScheduleWrite(const uint8_t command_type, const Serialization& serialization)
{
	const ScheduledWrite scheduled_write(command_type, serialization);
    if (!scheduled_writes_.full() && !scheduled_writes_.contains(scheduled_write)) {
		scheduled_writes_.push(scheduled_write);
	}
}

void SerialPort::issueWrite()
{
	if (!scheduled_writes_.empty()) {
		if (write_offset_ < scheduled_writes_.peek().serialization.size()) {
			const int remaining_bytes = scheduled_writes_.peek().serialization.size() - write_offset_;
			const uint8_t* buffer_offset = scheduled_writes_.peek().serialization.data().get() + write_offset_;
			const int status = SPP_Data_Write(Stack::instance().id(),
											  SPP::instance().id(),
											  remaining_bytes,
											  (Byte_t*) buffer_offset);
			if (status >= 0) {
				write_offset_ += status;
			}
			else {
				Error::instance().set(Error::TYPE_BT_SERIAL_PORT_DATA_WRITE, status);
			}
		}

		if (write_offset_ == scheduled_writes_.peek().serialization.size()) {
			scheduled_writes_.pop();
			write_offset_ = 0;
		}
	}
}

void SerialPort::progress()
{
	receiveData();
	issueWrite();
}

void SerialPort::clear()
{
	const int status = SPP_Purge_Buffer(
		Stack::instance().id(),
		SPP::instance().id(),
		SPP_PURGE_MASK_TRANSMIT_ABORT_BIT | SPP_PURGE_MASK_RECEIVE_ABORT_BIT);
	if (status == 0) {
		read_buffer_length_ = 0;
		scheduled_writes_.clear();
		write_offset_ = 0;
	}
	else {
		Error::instance().set(Error::TYPE_BT_SPP_CLEAR, status);
	}
}

void SerialPort::signalTransmitReady()
{
    issueWrite();
}

int SerialPort::bufferedReadLength()
{
    return read_buffer_length_;
}

int SerialPort::bufferedWriteLength()
{
    Error::instance().set(Error::TYPE_BT_SERIAL_PORT_WRITE_LEN, Error::no_info);
    return 0;
}

bool SerialPort::hasReadData()
{
    return bufferedReadLength() > 0;
}

bool SerialPort::hasWriteData()
{
    return bufferedWriteLength() > 0;
}

bool SerialPort::fits(const int write_data_size)
{
    Error::instance().set(Error::TYPE_BT_SERIAL_PORT_WRITE_FITS, Error::no_info);
    return false;
}
