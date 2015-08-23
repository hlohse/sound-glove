#include "SendCalibrationCommand.h"
#include "Platform.h"

SendCalibrationCommand::SendCalibrationCommand()
:	Command(Command::type_send_calibration)
{
}

void SendCalibrationCommand::execute(Context& context)
{
#ifdef PLATFORM_DESKTOP
	context.glove->setCalibration(calibration);
#endif
}

void SendCalibrationCommand::serialize(Serializer& serializer, Serialization& serialization, Context& context) const
{
	serializer.serialize((uint8_t*)calibration, maxDataSize());
}

void SendCalibrationCommand::deserialize(Deserializer& deserializer, Serialization& serialization, Context& context)
{
	deserializer.deserialize((uint8_t*)calibration, maxDataSize());
}

int SendCalibrationCommand::maxDataSize() const
{
	return max_data_size;
}
