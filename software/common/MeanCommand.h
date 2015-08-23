#ifndef MEANCOMMAND_H_
#define MEANCOMMAND_H_

#include "Command.h"
#include <stdint.h>

class MeanCommand : public Command {
public:
	static const uint8_t sensor_invalid = 0;
	static const uint8_t sensor_g       = 1;
	static const uint8_t sensor_a       = 2;
	static const uint8_t sensor_m       = 3;
	uint8_t sensor;
	uint32_t fifos;

	static const int max_data_size = sizeof(uint8_t) + sizeof(uint32_t);

	SOUNDGLOVE_API MeanCommand();

	// Command
	SOUNDGLOVE_API virtual void execute(Context& context);

protected:
	// Command
	virtual void serialize(Serializer& serializer, Serialization& serialization, Context& context) const;
	virtual void deserialize(Deserializer& deserializer, Serialization& serialization, Context& context);
	virtual int maxDataSize() const;
};

#endif /* MEANCOMMAND_H_ */
