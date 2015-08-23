#include "SendBatteryChargeCommand.h"
#include "Platform.h"

SendBatteryChargeCommand::SendBatteryChargeCommand()
:	Command(Command::type_send_battery_charge),
	battery_charge(0),
	is_used(false)
{
}

void SendBatteryChargeCommand::execute(Context& context)
{
#ifdef PLATFORM_DESKTOP
	context.glove->update(battery_charge, is_used != 0);
#endif
}

void SendBatteryChargeCommand::serialize(Serializer& serializer, Serialization& serialization, Context& context) const
{
	serializer.serialize((uint8_t*)&battery_charge, sizeof(battery_charge));
	serializer.serialize((uint8_t*)&is_used, sizeof(is_used));
}

void SendBatteryChargeCommand::deserialize(Deserializer& deserializer, Serialization& serialization, Context& context)
{
	deserializer.deserialize((uint8_t*)&battery_charge, sizeof(battery_charge));
	is_used = deserializer.deserialize();
}

int SendBatteryChargeCommand::maxDataSize() const
{
	return max_data_size;
}
