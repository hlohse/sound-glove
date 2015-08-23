#include "StartReadoutCommand.h"
#ifdef MSP430
#include "ReadoutCommand.h"
#include "TemperatureCommand.h"
#include "Scheduler.h"
#include "MARG.h"
#include "System.h"
#ifdef SYSTEM_LOG_LATENCY
#include "Timer.h"
#endif
#endif

#ifdef MSP430
void StartReadoutCommand::readout(void* parameters)
{
	Context& context = *((Context*) parameters);
	ReadoutCommand command;

#ifdef SYSTEM_LOG_LATENCY
	volatile float latency_readout = 0;
	volatile float latency_serial  = 0;
	Timer::start();
#endif

	command.execute(context);

#ifdef SYSTEM_LOG_LATENCY
	Timer::stop();
	latency_readout = Timer::duration;
	Timer::start();
#endif

	command.send(context);

#ifdef SYSTEM_LOG_LATENCY
	Timer::stop();
	latency_serial = Timer::duration;
#endif
}

void StartReadoutCommand::temperature(void* parameters)
{
	Context& context = *((Context*) parameters);
	TemperatureCommand command;
	command.execute(context);
	command.send(context);
}

bool& StartReadoutCommand::started()
{
	static bool instance;
	return instance;
}
#endif

StartReadoutCommand::StartReadoutCommand()
:	Command(Command::type_readout_start)
{
}

void StartReadoutCommand::execute(Context& context)
{
#ifdef MSP430
	if (!started()) {
		const int readout_period_ms = 1000 / MARG::defaultConfiguration().frequency;
		const int temperature_period_ms = 1000;

		Scheduler::instance().add(readout, &context, readout_period_ms);
		Scheduler::instance().add(temperature, &context, temperature_period_ms);

		started() = true;
	}
#endif
}

void StartReadoutCommand::serialize(Serializer& serializer, Serialization& serialization, Context& context) const
{
}

void StartReadoutCommand::deserialize(Deserializer& deserializer, Serialization& serialization, Context& context)
{
}

int StartReadoutCommand::maxDataSize() const
{
	return max_data_size;
}
