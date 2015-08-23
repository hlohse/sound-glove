#ifndef STOPWATCHCOMMAND_H_
#define STOPWATCHCOMMAND_H_

#include "Command.h"

class StopWatchCommand : public Command {
public:
    uint32_t stop_watch_ticks;

	static const int max_data_size = sizeof(uint32_t);

	SOUNDGLOVE_API StopWatchCommand();

	// Command
	SOUNDGLOVE_API virtual void execute(Context& context);

protected:
	// Command
	virtual void serialize(Serializer& serializer, Serialization& serialization, Context& context) const;
	virtual void deserialize(Deserializer& deserializer, Serialization& serialization, Context& context);
	virtual int maxDataSize() const;
};

#endif /* STOPWATCHCOMMAND_H_ */
