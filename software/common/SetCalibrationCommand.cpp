#include "SetCalibrationCommand.h"
#include "Platform.h"
#ifdef MSP430
#include "MARG.h"
#endif

SetCalibrationCommand::SetCalibrationCommand()
:	Command(Command::type_set_calibration)
{
}

void SetCalibrationCommand::execute(Context& context)
{
#ifdef MSP430
	MARG::hand().setCalibration(calibration[0]);
	MARG::forearm().setCalibration(calibration[1]);
	MARG::upperArm().setCalibration(calibration[2]);
#endif
}

void SetCalibrationCommand::serialize(Serializer& serializer, Serialization& serialization, Context& context) const
{
	serializer.serialize((uint8_t*)calibration, maxDataSize());
}

void SetCalibrationCommand::deserialize(Deserializer& deserializer, Serialization& serialization, Context& context)
{
	deserializer.deserialize((uint8_t*)calibration, maxDataSize());
}

int SetCalibrationCommand::maxDataSize() const
{
	return max_data_size;
}
