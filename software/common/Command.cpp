#include "Command.h"
#include "Platform.h"
#include "Deserializer.h"
#include "HandshakeCommand.h"
#include "ReadoutCommand.h"
#include "VibrationPeriodCommand.h"
#include "StopWatchCommand.h"
#include "TemperatureCommand.h"
#include "MeanCommand.h"
#include "MeanAnswer.h"
#include "SetCalibrationCommand.h"
#include "StartReadoutCommand.h"
#include "StopReadoutCommand.h"
#include "HeadingCommand.h"
#include "HeadingAnswer.h"
#include "SendCalibrationCommand.h"
#include "SendBatteryChargeCommand.h"
#include "DummyCommand.h"
#include "IntegrateGCommand.h"
#include "IntegrateGAnswer.h"
#ifdef MSP430
#include "Bluetooth/SerialPort.h"
#else
#include "../System.h"
#include <fstream>
using namespace std;
#endif

Command::Command(const uint8_t type)
:	type_(type)
{
}

bool Command::hasReceived(Context& context, const uint8_t type)
{
	return context.serial_port->bufferedReadLength() >= 1 ?
		type == context.serial_port->peek(0) :
		false;
}

SharedPointer<Command> Command::received(Context& context)
{
	if (context.serial_port->bufferedReadLength() < prefix_size) {
		return SharedPointer<Command>(new DummyCommand());
	}

	const uint8_t type = context.serial_port->peek(0);
	SharedPointer<Command> command;

	switch (type) {
		case type_handshake: {
			command = deserializedReceived<HandshakeCommand>(context, HandshakeCommand::max_data_size);
		} break;
		case type_readout: {
			command = deserializedReceived<ReadoutCommand>(context, ReadoutCommand::max_data_size);
		} break;
		case type_vibration_period: {
			command = deserializedReceived<VibrationPeriodCommand>(context, VibrationPeriodCommand::max_data_size);
		} break;
		case type_stop_watch: {
			command = deserializedReceived<StopWatchCommand>(context, StopWatchCommand::max_data_size);
		} break;
		case type_temperature: {
			command = deserializedReceived<TemperatureCommand>(context, TemperatureCommand::max_data_size);
		} break;
		case type_mean: {
			command = deserializedReceived<MeanCommand>(context, MeanCommand::max_data_size);
		} break;
		case type_mean_answer: {
			command = deserializedReceived<MeanAnswer>(context, MeanAnswer::max_data_size);
		} break;
		case type_set_calibration: {
			command = deserializedReceived<SetCalibrationCommand>(context, SetCalibrationCommand::max_data_size, true);
		} break;
		case type_readout_start: {
			command = deserializedReceived<StartReadoutCommand>(context, StartReadoutCommand::max_data_size);
		} break;
		case type_readout_stop: {
			command = deserializedReceived<StopReadoutCommand>(context, StopReadoutCommand::max_data_size);
		} break;
		case type_heading: {
			command = deserializedReceived<HeadingCommand>(context, HeadingCommand::max_data_size);
		} break;
		case type_heading_answer: {
			command = deserializedReceived<HeadingAnswer>(context, HeadingAnswer::max_data_size);
		} break;
		case type_send_calibration: {
			command = deserializedReceived<SendCalibrationCommand>(context, SendCalibrationCommand::max_data_size, true);
		} break;
		case type_send_battery_charge: {
			command = deserializedReceived<SendBatteryChargeCommand>(context, SendBatteryChargeCommand::max_data_size);
		} break;
		case type_integrate_g: {
			command = deserializedReceived<IntegrateGCommand>(context, IntegrateGCommand::max_data_size);
		} break;
		case type_integrate_g_answer: {
			command = deserializedReceived<IntegrateGAnswer>(context, IntegrateGAnswer::max_data_size);
		} break;
		default: {
			// Remove invalid unexpected data byte from buffer
			uint8_t discard;
			context.serial_port->read(&discard, 1);
			command = SharedPointer<Command>(new DummyCommand());
		} break;
	}

	return command;
}

template <typename T>
SharedPointer<Command> Command::deserializedReceived(Context& context, const int size, const bool force_use_size)
{
#ifdef READOUT_COMPRESS
	const int actual_data_size = force_use_size ? size : context.serial_port->peek(size_byte_index); // actual data size
	const int required_data = prefix_size + actual_data_size;
#else
	const int actual_data_size = size; // actual data size
	const int required_data = prefix_size + actual_data_size;
#endif

#ifdef MSP430
	// Don't block on glove, Bluetooth can't make progress otherwise
	if (context.serial_port->bufferedReadLength() < required_data) {
		return SharedPointer<Command>(new DummyCommand());
	}
#else
	while (context.serial_port->bufferedReadLength() < required_data) {
		context.serial_port->progress();
	}
#endif

    // Deserialize command from serial port data
	SharedPointer<Command> command = SharedPointer<Command>(new T());
	Serialization serialization(required_data);
	context.serial_port->read(serialization.data().get(), required_data);
	command->deserialize(serialization, context);

	return command;
}

void Command::send(Context& context) const
{
	if (context.serial_port->isOpen()) {
		const Serialization serialization = serialize(context);
#ifdef PLATFORM_DESKTOP
		context.serial_port->write(serialization.data().get(), serialization.size());
#elif MSP430
		Bluetooth::SerialPort& bt = *((Bluetooth::SerialPort*) context.serial_port);
		bt.write(type(), serialization);
#endif
	}
}

void Command::deserialize(Serialization& serialization, Context& context)
{
	Deserializer deserializer(serialization);
	type_ = deserializer.deserialize();
#ifdef READOUT_COMPRESS
	// Remove size from data
	const uint8_t size = deserializer.deserialize();
#ifdef PLATFORM_DESKTOP
#ifdef LOG_COMPRESSED_READOUT_SIZES
	if (type_ == Command::type_readout) {
		ofstream file("packet_sizes.csv", ios_base::app);
		if (file) {
			file << (int)size << endl;
			file.close();
		}
	}
#endif
#endif
#endif
	deserialize(deserializer, serialization, context);
}

Serialization Command::serialize(Context& context) const
{
	Serialization serialization(prefix_size + maxDataSize());
	Serializer serializer(serialization);
	serializer.serialize(type());
#ifdef READOUT_COMPRESS
	serializer.serialize((uint8_t)0);
	serialize(serializer, serialization, context);
	serialization.setSize(serializer.size());
	serialization.data().get()[size_byte_index] = serializer.size() - prefix_size;
#else
	serialize(serializer, serialization, context);
#endif
	return serialization;
}

uint8_t Command::type() const
{
	return type_;
}
