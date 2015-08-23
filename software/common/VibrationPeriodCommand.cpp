#include "VibrationPeriodCommand.h"
#include "Platform.h"
#ifdef MSP430
#include "Scheduler.h"
#include "VibrationMotor.h"
#endif

VibrationPeriodCommand::VibrationPeriodCommand()
:	Command(Command::type_vibration_period),
	period(512)
{
}

void VibrationPeriodCommand::execute(Context& context)
{
#ifdef MSP430
	Scheduler& scheduler = Scheduler::instance();
	scheduler.remove(VibrationMotor::instancePwm, NULL);
	scheduler.add(VibrationMotor::instancePwm, NULL, (int) period);
#endif
}

void VibrationPeriodCommand::serialize(Serializer& serializer, Serialization& serialization, Context& context) const
{
	serializer.serialize((uint8_t*)&period, sizeof(period));
}

void VibrationPeriodCommand::deserialize(Deserializer& deserializer, Serialization& serialization, Context& context)
{
	deserializer.deserialize((uint8_t*)&period, sizeof(period));
}

int VibrationPeriodCommand::maxDataSize() const
{
	return max_data_size;
}
