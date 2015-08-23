#ifndef HEADINGCOMMAND_H_
#define HEADINGCOMMAND_H_

#include "Command.h"

class HeadingCommand : public Command {
public:
	static const int max_data_size = 0;

	SOUNDGLOVE_API HeadingCommand();

	// Command
	SOUNDGLOVE_API virtual void execute(Context& context);

protected:
	static const int fifos = 100;

	// Command
	virtual void serialize(Serializer& serializer, Serialization& serialization, Context& context) const;
	virtual void deserialize(Deserializer& deserializer, Serialization& serialization, Context& context);
	virtual int maxDataSize() const;
};

#endif /* HEADINGCOMMAND_H_ */
