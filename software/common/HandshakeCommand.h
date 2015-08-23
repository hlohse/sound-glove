#ifndef HANDSHAKE_H_
#define HANDSHAKE_H_

#include "Command.h"
#include <stdint.h>

class HandshakeCommand : public Command {
public:
	static const int max_data_size = 65;

	SOUNDGLOVE_API HandshakeCommand();

	// Command
	SOUNDGLOVE_API virtual void execute(Context& context);

protected:
    // Command
	virtual void serialize(Serializer& serializer, Serialization& serialization, Context& context) const;
	virtual void deserialize(Deserializer& deserializer, Serialization& serialization, Context& context);
	virtual int maxDataSize() const;

private:
	uint8_t emulated_readout_[65];
};

#endif /* HANDSHAKE_H_ */
