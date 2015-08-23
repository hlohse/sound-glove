#include "HandshakeCommand.h"

HandshakeCommand::HandshakeCommand()
:	Command(Command::type_handshake),
	emulated_readout_()
{
}

void HandshakeCommand::execute(Context& context)
{
}

void HandshakeCommand::deserialize(Deserializer& deserializer, Serialization& serialization, Context& context)
{
	deserializer.deserialize(emulated_readout_, maxDataSize());
}

void HandshakeCommand::serialize(Serializer& serializer, Serialization& serialization, Context& context) const
{
	serializer.serialize((uint8_t*)emulated_readout_, maxDataSize());
}

int HandshakeCommand::maxDataSize() const
{
	return max_data_size;
}
