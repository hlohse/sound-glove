#include "TemperatureCommand.h"
#include "ReadoutCommand.h"
#include "Platform.h"
#ifdef MSP430
#include "MARG.h"
#endif
#ifdef PLATFORM_DESKTOP
#include "../desktop/soundglove/Temperature.h"
#include "LSM9DS0.h"
#endif

/* Ignore "warning C4351: new behaviour: elements of array 'name' will be
 * default initialized".
 */
#ifdef OS_WINDOWS
#pragma warning( push )
#pragma warning( disable : 4351 )
#endif

TemperatureCommand::TemperatureCommand()
:	Command(Command::type_temperature),
	temperature()
{
}

void TemperatureCommand::execute(Context& context)
{
#ifdef MSP430
	temperature[0] = ReadoutCommand::readout.margs[0].t;
	temperature[1] = ReadoutCommand::readout.margs[1].t;
	temperature[2] = ReadoutCommand::readout.margs[2].t;
#endif
#ifdef PLATFORM_DESKTOP
	context.glove->update(temperature);
#endif
}

void TemperatureCommand::serialize(Serializer& serializer, Serialization& serialization, Context& context) const
{
	serializer.serialize((uint8_t*)temperature, 6);
}

void TemperatureCommand::deserialize(Deserializer& deserializer, Serialization& serialization, Context& context)
{
	deserializer.deserialize((uint8_t*)temperature, sizeof(temperature));
}

int TemperatureCommand::maxDataSize() const
{
	return max_data_size;
}
