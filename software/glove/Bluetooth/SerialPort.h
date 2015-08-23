#ifndef BLUETOOTH_SERIALPORT_H_
#define BLUETOOTH_SERIALPORT_H_

// Use defines to avoid VLA memory pool allocations
#define BLUETOOTH_SERIAL_PORT_READ_BUFFER_SIZE     320
#define BLUETOOTH_SERIAL_PORT_MAX_SCHEDULED_WRITES   8

#include "ISerialPort.h"
#include "Serialization.h"
#include "RingBuffer.h"

namespace Bluetooth {

class SerialPort : public ISerialPort {
    friend class SPP;

public:
    static SerialPort& instance();

    void write(const uint8_t command_type, const Serialization& serialization);

    // ISerialPort
    virtual bool isOpen();

	virtual uint8_t peek(const int index);

    virtual void read(uint8_t* destination, const int length);
    virtual void write(uint8_t* source, const int length);

	virtual void progress();

    virtual void clear();

    virtual int bufferedReadLength();
    virtual int bufferedWriteLength();

    virtual bool hasReadData();
    virtual bool hasWriteData();

    virtual bool fits(const int write_data_size);

private:
    struct ScheduledWrite {
    	uint8_t command_type;
    	Serialization serialization;
    	ScheduledWrite();
    	ScheduledWrite(uint8_t command_type, const Serialization& serialization);
    	bool operator==(const ScheduledWrite& other) const;
    };

    RingBuffer<ScheduledWrite, BLUETOOTH_SERIAL_PORT_MAX_SCHEDULED_WRITES> scheduled_writes_;
    uint8_t read_buffer_[BLUETOOTH_SERIAL_PORT_READ_BUFFER_SIZE];
    int read_buffer_length_;
    int write_offset_;

    SerialPort();

    void tryScheduleWrite(const uint8_t command_type, const Serialization& serialization);
    void issueWrite();

    // Called by SPP
    void receiveData();
    void signalTransmitReady();

    // Disallow copies; (do not implement!)
    SerialPort(const SerialPort&);
    void operator=(const SerialPort&);
};

}

#endif /* BLUETOOTH_SERIALPORT_H_ */
