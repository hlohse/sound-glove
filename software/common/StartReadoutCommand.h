#ifndef STARTREADOUTCOMMAND_H_
#define STARTREADOUTCOMMAND_H_

#include "Command.h"
#include "Platform.h"

class StartReadoutCommand : public Command {
	friend class StopReadoutCommand;

public:
	static const int max_data_size = 0;

	SOUNDGLOVE_API StartReadoutCommand();

	// Command
	SOUNDGLOVE_API virtual void execute(Context& context);

protected:
#ifdef MSP430
	static void readout(void* parameters);
	static void temperature(void* parameters);

	static bool& started();
#endif
	
	// Command
	virtual void serialize(Serializer& serializer, Serialization& serialization, Context& context) const;
	virtual void deserialize(Deserializer& deserializer, Serialization& serialization, Context& context);
	virtual int maxDataSize() const;
};
#endif /* STARTREADOUTCOMMAND_H_ */
