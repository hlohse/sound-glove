#ifndef VIBRATIONPERIODCOMMAND_H_
#define VIBRATIONPERIODCOMMAND_H_

#include "Command.h"
#include <stdint.h>

class VibrationPeriodCommand : public Command {
public:
	uint16_t period;

	static const int max_data_size = sizeof(uint16_t);

	SOUNDGLOVE_API VibrationPeriodCommand();

	// Command
	SOUNDGLOVE_API virtual void execute(Context& context);

protected:
	// Command
	virtual void serialize(Serializer& serializer, Serialization& serialization, Context& context) const;
	virtual void deserialize(Deserializer& deserializer, Serialization& serialization, Context& context);
	virtual int maxDataSize() const;
};

#endif /* VIBRATIONPERIODCOMMAND_H_ */
