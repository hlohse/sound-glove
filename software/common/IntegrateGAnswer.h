#ifndef INTEGRATE_G_ANSWER_H_
#define INTEGRATE_G_ANSWER_H_

#include "Command.h"
#include "Vector.h"
#include <stdint.h>

class IntegrateGAnswer : public Command {
public:
	Vector<float> rotation;

	static const int max_data_size = sizeof(Vector<float>);

	SOUNDGLOVE_API IntegrateGAnswer();

	// Command
	SOUNDGLOVE_API virtual void execute(Context& context);

protected:
	// Command
	virtual void serialize(Serializer& serializer, Serialization& serialization, Context& context) const;
	virtual void deserialize(Deserializer& deserializer, Serialization& serialization, Context& context);
	virtual int maxDataSize() const;
};



#endif // INTEGRATE_G_ANSWER_H_
