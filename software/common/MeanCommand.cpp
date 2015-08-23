#include "MeanCommand.h"
#include "Platform.h"
#ifdef MSP430
#include "MeanAnswer.h"
#include "MARG.h"
#endif

MeanCommand::MeanCommand()
:	Command(Command::type_mean),
	sensor(sensor_invalid),
	fifos(0)
{
}

void MeanCommand::execute(Context& context)
{
#ifdef MSP430
	MeanAnswer answer;
	switch (sensor) {
	case sensor_g: {
		answer.sensor_calibration[0] = MARG::hand().determineMeanG(fifos);
		answer.sensor_calibration[1] = MARG::forearm().determineMeanG(fifos);
		answer.sensor_calibration[2] = MARG::upperArm().determineMeanG(fifos);
	} break;
	case sensor_a: {
		answer.sensor_calibration[0] = MARG::hand().determineMeanA(fifos);
		answer.sensor_calibration[1] = MARG::forearm().determineMeanA(fifos);
		answer.sensor_calibration[2] = MARG::upperArm().determineMeanA(fifos);
	} break;
	case sensor_m: {
		answer.sensor_calibration[0] = MARG::hand().determineMeanM(fifos);
		answer.sensor_calibration[1] = MARG::forearm().determineMeanM(fifos);
		answer.sensor_calibration[2] = MARG::upperArm().determineMeanM(fifos);
	} break;
	default: break;
	}
	answer.send(context);
#endif
}

void MeanCommand::serialize(Serializer& serializer, Serialization& serialization, Context& context) const
{
	serializer.serialize(sensor);
	serializer.serialize((uint8_t*)&fifos, sizeof(fifos));
}

void MeanCommand::deserialize(Deserializer& deserializer, Serialization& serialization, Context& context)
{
	sensor = deserializer.deserialize();
	deserializer.deserialize((uint8_t*)&fifos, sizeof(fifos));
}

int MeanCommand::maxDataSize() const
{
	return max_data_size;
}
