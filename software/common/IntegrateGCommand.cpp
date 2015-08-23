#include "IntegrateGCommand.h"
#include "Platform.h"
#ifdef MSP430
#include "IntegrateGAnswer.h"
#include "MARG.h"
#include <stdlib.h>
#endif

IntegrateGCommand::IntegrateGCommand()
:   Command(type_integrate_g),
	sensor(0),
    duration_s(0),
	calibration_offset()
{
}

void IntegrateGCommand::execute(Context& context)
{
#ifdef MSP430
	IntegrateGAnswer answer;
	MARG* marg = NULL;
	answer.rotation = Vector<float>(0, 0, 0);

	switch (sensor) {
	case sensor_hand: {
		marg = &MARG::hand();
	} break;
	case sensor_forearm: {
		marg = &MARG::forearm();
	} break;
	case sensor_upper_arm: {
		marg = &MARG::upperArm();
	} break;
	default: break;
	}

	if (marg != NULL && marg->isDoneReadingOut()) {
		const uint32_t samples = ((uint32_t)duration_s) * ((uint32_t)g_odr);
		for (uint32_t i = 0; i < samples; ++i) {
			while (!marg->hasNewG());
			const Vector<int16_t> readout = marg->getG() - calibration_offset;
			const Vector<float> degrees = Vector<float>(
				((float)readout.x()),
				((float)readout.y()),
				((float)readout.z())
			) * ((float)g_mdps_per_digit) / 1000;
			answer.rotation += degrees / g_odr;
		}
	}

	answer.send(context);
#endif
}

void IntegrateGCommand::serialize(Serializer& serializer, Serialization& serialization, Context& context) const
{
	serializer.serialize(sensor);
	serializer.serialize(duration_s);
	serializer.serialize((uint8_t*)calibration_offset.values, sizeof(calibration_offset.values));
}

void IntegrateGCommand::deserialize(Deserializer& deserializer, Serialization& serialization, Context& context)
{
	sensor = deserializer.deserialize();
	duration_s = deserializer.deserialize();
	deserializer.deserialize((uint8_t*)calibration_offset.values, sizeof(calibration_offset.values));
}

int IntegrateGCommand::maxDataSize() const
{
	return max_data_size;
}
