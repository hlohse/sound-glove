#ifndef READOUTCOMMAND_H_
#define READOUTCOMMAND_H_

#include "Command.h"
#include "Platform.h"
#include "Readout.h"

class ReadoutCommand : public Command {
public:
#ifdef PLATFORM_DESKTOP
	Readout readout;
#else
	static Readout readout;
#endif

#ifdef READOUT_COMPRESS
	// 1 bit diff indicator per vector value => + 3*3*3+5 bits = 32 bits = 4 bytes
	static const int max_data_size = 3 * (sizeof(Readout::MARG) - sizeof(int16_t)) + 5 * sizeof(uint16_t) + 1 + 4;
#else
	// Don't transmit temperature (done separately via TemperatureReadoutCommand)
	static const int max_data_size = 3 * (sizeof(Readout::MARG) - sizeof(int16_t)) + 5 * sizeof(uint16_t) + 1;
#endif

	SOUNDGLOVE_API ReadoutCommand();

	// Command
	SOUNDGLOVE_API virtual void execute(Context& context);

protected:
	static const uint8_t flag_uncompressed = 0;
	static const uint8_t flag_compressed = 1;

	// Command
	virtual void serialize(Serializer& serializer, Serialization& serialization, Context& context) const;
	virtual void deserialize(Deserializer& deserializer, Serialization& serialization, Context& context);
	virtual int maxDataSize() const;

private:
#ifdef MSP430
	// Replaces call to log2
	uint8_t requiredBits(const uint8_t value) const;
#endif

	void serialize(Serializer& serializer,
				   Serialization& serialization,
				   const Vector<int16_t>& values,
				   const Vector<int16_t>& diff_ref) const;
	void serialize(Serializer& serializer,
		           Serialization& serialization,
				   const int16_t value,
				   const int16_t diff_ref) const;

	void deserialize(Deserializer& deserializer,
		Serialization& serialization,
		Vector<int16_t>& values,
		const Vector<int16_t>& diff_ref) const;
	int16_t deserialize(Deserializer& deserializer,
		Serialization& serialization,
		const int16_t diff_ref) const;

	void serialize(Serializer& serializer, const Readout::MARG& marg) const;
	void deserialize(Deserializer& deserializer, Readout::MARG& marg);
};

#endif /* READOUTCOMMAND_H_ */
