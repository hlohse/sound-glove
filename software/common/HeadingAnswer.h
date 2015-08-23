#ifndef HEADINGANSWER_H_
#define HEADINGANSWER_H_

#include "Command.h"
#include "Vector.h"
#include <stdint.h>

class HeadingAnswer : public Command {
public:
	Vector<int16_t> a[3];
	Vector<int16_t> m[3];

	static const int max_data_size = 2 * 3 * sizeof(Vector<int16_t>);

	SOUNDGLOVE_API HeadingAnswer();

	// Command
	SOUNDGLOVE_API virtual void execute(Context& context);

protected:
	// Command
	virtual void serialize(Serializer& serializer, Serialization& serialization, Context& context) const;
	virtual void deserialize(Deserializer& deserializer, Serialization& serialization, Context& context);
	virtual int maxDataSize() const;
};

#endif /* HEADINGANSWER_H_ */
