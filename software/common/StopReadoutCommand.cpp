#include "StopReadoutCommand.h"
#include "StartReadoutCommand.h"
#include "Platform.h"
#ifdef MSP430
#include "Scheduler.h"
#endif

StopReadoutCommand::StopReadoutCommand()
:	Command(Command::type_readout_stop)
{
}

void StopReadoutCommand::execute(Context& context)
{
#ifdef MSP430
	if (StartReadoutCommand::started()) {
		const int wait_period_ms = 50;
		context.serial_port->clear();
		Scheduler::instance().remove(StartReadoutCommand::readout, &context);
		Scheduler::instance().remove(StartReadoutCommand::temperature, &context);
		// Wait a moment in case pending data arrives
		__delay_cycles((25e6 * wait_period_ms) / 1000);
		context.serial_port->clear();
		StartReadoutCommand::started() = false;
	}
#endif
}

void StopReadoutCommand::serialize(Serializer& serializer, Serialization& serialization, Context& context) const
{
}

void StopReadoutCommand::deserialize(Deserializer& deserializer, Serialization& serialization, Context& context)
{
}

int StopReadoutCommand::maxDataSize() const
{
	return max_data_size;
}
