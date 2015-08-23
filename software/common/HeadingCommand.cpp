#include "HeadingCommand.h"
#include "Platform.h"
#ifdef MSP430
#include "HeadingAnswer.h"
#include "MARG.h"
#endif

HeadingCommand::HeadingCommand()
:	Command(Command::type_heading)
{
}

void HeadingCommand::execute(Context& context)
{
#ifdef MSP430
	HeadingAnswer answer;
	answer.a[0] = MARG::hand().determineMeanA(fifos);
	answer.a[1] = MARG::forearm().determineMeanA(fifos);
	answer.a[2] = MARG::upperArm().determineMeanA(fifos);
	answer.m[0] = MARG::hand().determineMeanM(fifos);
	answer.m[1] = MARG::forearm().determineMeanM(fifos);
	answer.m[2] = MARG::upperArm().determineMeanM(fifos);
	answer.send(context);
#endif
}

void HeadingCommand::serialize(Serializer& serializer, Serialization& serialization, Context& context) const
{
}

void HeadingCommand::deserialize(Deserializer& deserializer, Serialization& serialization, Context& context)
{
}

int HeadingCommand::maxDataSize() const
{
	return max_data_size;
}
