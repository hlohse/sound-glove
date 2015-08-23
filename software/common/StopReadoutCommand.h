#ifndef STOPREADOUTCOMMAND_H_
#define STOPREADOUTCOMMAND_H_

#include "Command.h"

class StopReadoutCommand : public Command {
public:
	static const int max_data_size = 0;

	SOUNDGLOVE_API StopReadoutCommand();

	// Command
	SOUNDGLOVE_API virtual void execute(Context& context);

protected:
	// Command
	virtual void serialize(Serializer& serializer, Serialization& serialization, Context& context) const;
	virtual void deserialize(Deserializer& deserializer, Serialization& serialization, Context& context);
	virtual int maxDataSize() const;
};
#endif /* STOPREADOUTCOMMAND_H_ */
