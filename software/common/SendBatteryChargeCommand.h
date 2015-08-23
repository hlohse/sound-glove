#ifndef SENDBATTERYCHARGECOMMAND_H_
#define SENDBATTERYCHARGECOMMAND_H_

#include "Command.h"
#include <stdint.h>

class SendBatteryChargeCommand : public Command {
public:
	uint16_t battery_charge;
	uint8_t is_used;

	static const int max_data_size = sizeof(uint16_t) + sizeof(uint8_t);

	SOUNDGLOVE_API SendBatteryChargeCommand();

	// Command
	SOUNDGLOVE_API virtual void execute(Context& context);

protected:
	// Command
	virtual void serialize(Serializer& serializer, Serialization& serialization, Context& context) const;
	virtual void deserialize(Deserializer& deserializer, Serialization& serialization, Context& context);
	virtual int maxDataSize() const;
};

#endif /* SENDBATTERYCHARGECOMMAND_H_ */
