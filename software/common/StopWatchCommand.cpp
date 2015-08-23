#include "StopWatchCommand.h"
#include "Platform.h"
#ifdef MSP430
#include <Timer.h>
#endif
#ifdef PLATFORM_DESKTOP
#include <iostream>
using namespace std;
#endif

StopWatchCommand::StopWatchCommand()
:	Command(Command::type_stop_watch),
	stop_watch_ticks(0)
{
}

void StopWatchCommand::execute(Context& context)
{
#ifdef MSP430
	Timer::stop();
	stop_watch_ticks = 0;
#endif
#ifdef PLATFORM_DESKTOP
	const double us_per_tick = 1000000.0 / 32768.0; // ACLK period in us
	const double time_ms = ((double) stop_watch_ticks) * us_per_tick / 1000.0;
	cout << "StopWatch: " << time_ms << "ms" << endl;
#endif
}

void StopWatchCommand::serialize(Serializer& serializer, Serialization& serialization, Context& context) const
{
	serializer.serialize((uint8_t*)&stop_watch_ticks, sizeof(stop_watch_ticks));
}

void StopWatchCommand::deserialize(Deserializer& deserializer, Serialization& serialization, Context& context)
{
	deserializer.deserialize((uint8_t*)&stop_watch_ticks, sizeof(stop_watch_ticks));
}

int StopWatchCommand::maxDataSize() const
{
	return max_data_size;
}
