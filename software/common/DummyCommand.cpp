#include "DummyCommand.h"

DummyCommand::DummyCommand()
:	Command(Command::type_invalid)
{
}

void DummyCommand::execute(Context& context)
{
}

void DummyCommand::deserialize(Deserializer& deserializer, Serialization& serialization, Context& context)
{
}

void DummyCommand::serialize(Serializer& serializer, Serialization& serialization, Context& context) const
{
}

int DummyCommand::maxDataSize() const
{
	return max_data_size;
}
