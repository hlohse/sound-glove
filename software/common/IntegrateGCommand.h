#ifndef INTEGRATE_G_COMMAND_H_
#define INTEGRATE_G_COMMAND_H_

#include "Command.h"
#include <stdint.h>

class IntegrateGCommand : public Command {
public:
	static const int g_odr = 760;
	static const int g_mdps_per_digit = 70;

	static const uint8_t sensor_hand      = 1;
	static const uint8_t sensor_forearm = 2;
	static const uint8_t sensor_upper_arm = 3;

	uint8_t sensor;
	uint8_t duration_s; // Duration in seconds
	Vector<int16_t> calibration_offset;

	static const int max_data_size = 2 * sizeof(uint8_t) + sizeof(Vector<int16_t>);

	SOUNDGLOVE_API IntegrateGCommand();

	// Command
	SOUNDGLOVE_API virtual void execute(Context& context);

protected:
	// Command
	virtual void serialize(Serializer& serializer, Serialization& serialization, Context& context) const;
	virtual void deserialize(Deserializer& deserializer, Serialization& serialization, Context& context);
	virtual int maxDataSize() const;
};



#endif // INTEGRATE_G_COMMAND_H_
