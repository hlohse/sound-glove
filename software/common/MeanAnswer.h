#ifndef MEANANSWER_H_
#define MEANANSWER_H_

#include "Command.h"
#include "Vector.h"

class MeanAnswer : public Command {
public:
    Vector<int16_t> sensor_calibration[3];

	static const int max_data_size = 3 * sizeof(Vector<int16_t>);

	SOUNDGLOVE_API MeanAnswer();

	// Command
	SOUNDGLOVE_API virtual void execute(Context& context);

protected:
	// Command
	virtual void serialize(Serializer& serializer, Serialization& serialization, Context& context) const;
	virtual void deserialize(Deserializer& deserializer, Serialization& serialization, Context& context);
	virtual int maxDataSize() const;
};

#endif /* MEANANSWER_H_ */
