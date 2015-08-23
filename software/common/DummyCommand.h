#ifndef DUMMYCOMMAND_H_
#define DUMMYCOMMAND_H_

#include "Command.h"

class DummyCommand : public Command {
public:
	static const int max_data_size = 0;

	SOUNDGLOVE_API DummyCommand();

	// Command
	SOUNDGLOVE_API virtual void execute(Context& context);

protected:
    // Command
	virtual void serialize(Serializer& serializer, Serialization& serialization, Context& context) const;
	virtual void deserialize(Deserializer& deserializer, Serialization& serialization, Context& context);
	virtual int maxDataSize() const;
};

#endif /* DUMMYCOMMAND_H_ */
