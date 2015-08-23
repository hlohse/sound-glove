#ifndef COMMAND_H_
#define COMMAND_H_

#include "Export.h"
#include "Platform.h"
#include "ISerializeable.h"
#include "ISerialPort.h"
#include "Context.h"
#include "SharedPointer.h"
#include "Serializer.h"
#include "Deserializer.h"
#include "Serialization.h"

class Command : public ISerializeable {
public:
#ifdef READOUT_COMPRESS
	static const int prefix_size = 2; // type + size
#else
	static const int prefix_size = 1; // type
#endif

	// Byte variables instead of enum to guarantee equal size on platforms
	static const uint8_t type_invalid             =  0;
	static const uint8_t type_handshake           =  1;
	static const uint8_t type_readout             =  2;
	static const uint8_t type_vibration_period    =  3;
	static const uint8_t type_stop_watch          =  4;
	static const uint8_t type_temperature         =  5;
	static const uint8_t type_mean                =  6;
	static const uint8_t type_mean_answer         =  7;
	static const uint8_t type_set_calibration     =  8;
	static const uint8_t type_readout_start       =  9;
	static const uint8_t type_readout_stop        = 10;
	static const uint8_t type_heading             = 11;
	static const uint8_t type_heading_answer      = 12;
	static const uint8_t type_send_calibration    = 13;
	static const uint8_t type_send_battery_charge = 14;
	static const uint8_t type_integrate_g         = 15;
	static const uint8_t type_integrate_g_answer  = 16;

	Command(const uint8_t type);

	SOUNDGLOVE_API static bool hasReceived(Context& context, const uint8_t type);
	SOUNDGLOVE_API static SharedPointer<Command> received(Context& context);

	SOUNDGLOVE_API virtual void execute(Context& context) = 0;
	SOUNDGLOVE_API void send(Context& context) const;

	SOUNDGLOVE_API uint8_t type() const;

protected:
	static const int size_byte_index = 1;

	uint8_t type_;

	// ISerializeable
	Serialization serialize(Context& context) const;
	void deserialize(Serialization& serialization, Context& context);

	virtual void serialize(Serializer& serializer, Serialization& serialization, Context& context) const = 0;
	virtual void deserialize(Deserializer& deserializer, Serialization& serialization, Context& context) = 0;
    virtual int maxDataSize() const = 0;

private:
	template <typename T>
	static SharedPointer<Command> deserializedReceived(Context& context, const int size, const bool force_use_size = false);
};

#endif /* COMMAND_H_ */
