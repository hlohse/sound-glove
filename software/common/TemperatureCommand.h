#ifndef TEMPERATURECOMMAND_H_
#define TEMPERATURECOMMAND_H_

#include "Command.h"
#include "Readout.h"

class TemperatureCommand : public Command {
public:
    int16_t temperature[3];

	static const int max_data_size = 3 * sizeof(int16_t);

	SOUNDGLOVE_API TemperatureCommand();

	// Command
	SOUNDGLOVE_API virtual void execute(Context& context);

protected:
	// Command
	virtual void serialize(Serializer& serializer, Serialization& serialization, Context& context) const;
	virtual void deserialize(Deserializer& deserializer, Serialization& serialization, Context& context);
	virtual int maxDataSize() const;
};

#endif /* TEMPERATURECOMMAND_H_ */
