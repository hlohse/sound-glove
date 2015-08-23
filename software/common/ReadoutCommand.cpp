#include "ReadoutCommand.h"
#include <math.h>
#ifdef MSP430
#include "MARG.h"
#endif

#ifdef MSP430
Readout ReadoutCommand::readout;
#endif

ReadoutCommand::ReadoutCommand()
:	Command(Command::type_readout)
{
}

void ReadoutCommand::execute(Context& context)
{
#ifdef MSP430
	MARG::hand().startReadingOut(&(readout.margs[0]));
	MARG::forearm().startReadingOut(&(readout.margs[1]));
	MARG::upperArm().startReadingOut(&(readout.margs[2]));
    while (!MARG::hand().isDoneReadingOut()
    ||     !MARG::forearm().isDoneReadingOut()
    ||     !MARG::upperArm().isDoneReadingOut())
    {
		LPM0;
    }
	readout.capture();
#else
	context.glove->update(readout);
#endif
}

void ReadoutCommand::serialize(Serializer& serializer, Serialization& serialization, Context& context) const
{
	const Readout& r = readout;

#ifdef READOUT_COMPRESS
	for (int i = 0; i < 3; ++i) {
		serialize(serializer, serialization, r.margs[i].g, context.readout_diff_ref->margs[i].g);
		serialize(serializer, serialization, r.margs[i].a, context.readout_diff_ref->margs[i].a);
		serialize(serializer, serialization, r.margs[i].m, context.readout_diff_ref->margs[i].m);
		context.readout_diff_ref->margs[i] = r.margs[i];
	}
	for (int i = 0; i < 5; ++i) {
		serialize(serializer, serialization, r.flex[i], context.readout_diff_ref->flex[i]);
		context.readout_diff_ref->flex[i] = r.flex[i];
	}
	serializer.serialize(r.is_touched, 1);
#else
	for (int i = 0; i < 3; ++i) {
		serialize(serializer, r.margs[i]);
	}
	serializer.serialize((uint8_t*)r.flex, sizeof(r.flex));
	serializer.serialize(r.is_touched);
#endif
}

void ReadoutCommand::deserialize(Deserializer& deserializer, Serialization& serialization, Context& context)
{
	Readout& r = readout;

#ifdef READOUT_COMPRESS
	for (int i = 0; i < 3; ++i) {
		deserialize(deserializer, serialization, r.margs[i].g, context.readout_diff_ref->margs[i].g);
		deserialize(deserializer, serialization, r.margs[i].a, context.readout_diff_ref->margs[i].a);
		deserialize(deserializer, serialization, r.margs[i].m, context.readout_diff_ref->margs[i].m);
		context.readout_diff_ref->margs[i] = r.margs[i];
	}
	for (int i = 0; i < 5; ++i) {
		r.flex[i] = deserialize(deserializer, serialization, context.readout_diff_ref->flex[i]);
		context.readout_diff_ref->flex[i] = r.flex[i];
	}
	r.is_touched = deserializer.deserialize(1);
#else
	for (int i = 0; i < 3; ++i) {
		deserialize(deserializer, r.margs[i]);
	}
	deserializer.deserialize((uint8_t*)r.flex, sizeof(r.flex));
	r.is_touched = deserializer.deserialize();
#endif
}

void ReadoutCommand::serialize(Serializer& serializer,
							   Serialization& serialization,
							   const Vector<int16_t>& values,
							   const Vector<int16_t>& diff_ref) const
{
	serialize(serializer, serialization, values.x(), diff_ref.x());
	serialize(serializer, serialization, values.y(), diff_ref.y());
	serialize(serializer, serialization, values.z(), diff_ref.z());
}

void ReadoutCommand::serialize(Serializer& serializer,
							   Serialization& serialization,
							   const int16_t value,
							   const int16_t diff_ref) const
{
	const uint8_t high_diff = (value ^ diff_ref) & ~((1 << READOUT_COMPRESS_LSB) - 1);
	const bool high_changed = high_diff != 0;
	const bool sign_changed = value < 0 && diff_ref >= 0 || value >= 0 && diff_ref < 0;
	if (high_changed || sign_changed) {
		serializer.serialize(flag_uncompressed, 1);
		serializer.serialize((uint8_t*)&value, 2);
	}
	else {
		const uint8_t diff = (value ^ diff_ref) & ((1 << READOUT_COMPRESS_LSB) - 1);
		const uint8_t diff_bits = diff != 0 ?
#ifdef MSP430
			requiredBits(diff) :
#else
			(uint8_t)log2((float)abs(diff)) + 1:
#endif
			1; // Assumption: Values marginally equal, therefore always transmit 1 byte
		serializer.serialize(flag_compressed, 1);
		serializer.serialize(diff_bits - 1, READOUT_COMPRESS_LSB); // E.g. 1..8 bits -> 0..7 = 000...111
		serializer.serialize(diff, diff_bits);
	}
}

void ReadoutCommand::deserialize(Deserializer& deserializer,
								 Serialization& serialization,
								 Vector<int16_t>& values,
								 const Vector<int16_t>& diff_ref) const
{
	values.setX(deserialize(deserializer, serialization, diff_ref.x()));
	values.setY(deserialize(deserializer, serialization, diff_ref.y()));
	values.setZ(deserialize(deserializer, serialization, diff_ref.z()));
}

int16_t ReadoutCommand::deserialize(Deserializer& deserializer,
									Serialization& serialization,
									const int16_t diff_ref) const
{
	const uint8_t compression_flag = deserializer.deserialize(1);
	const bool is_compressed = compression_flag == flag_compressed;
	int16_t value = 0;
	if (is_compressed) {
		const uint8_t diff_bits = deserializer.deserialize(READOUT_COMPRESS_LSB) + 1;
		const uint8_t diff_lsbs = deserializer.deserialize(diff_bits);
		const uint16_t mask_out_lsbs = ~((1 << diff_bits) - 1);
		value = (diff_ref & mask_out_lsbs) | diff_lsbs;
	}
	else {
		deserializer.deserialize((uint8_t*)&value, 2);
	}
	return value;
}

void ReadoutCommand::serialize(Serializer& serializer, const Readout::MARG& marg) const
{
	serializer.serialize((uint8_t*)marg.g.values, 6);
	serializer.serialize((uint8_t*)marg.a.values, 6);
	serializer.serialize((uint8_t*)marg.m.values, 6);
	// Don't serialize temperature to keep size small
}

void ReadoutCommand::deserialize(Deserializer& deserializer, Readout::MARG& marg)
{
	deserializer.deserialize((uint8_t*)marg.g.values, sizeof(marg.g.values));
	deserializer.deserialize((uint8_t*)marg.a.values, sizeof(marg.a.values));
	deserializer.deserialize((uint8_t*)marg.m.values, sizeof(marg.m.values));
}

int ReadoutCommand::maxDataSize() const
{
	return max_data_size;
}

#ifdef MSP430
uint8_t ReadoutCommand::requiredBits(const uint8_t value) const
{
	if (value >= 2048) {
		return 12;
	}
	else if (value >= 1024) {
		return 11;
	}
	else if (value >= 512) {
		return 10;
	}
	else if (value >= 256) {
		return 9;
	}
	else if (value >= 128) {
		return 8;
	}
	else if (value >= 64) {
		return 7;
	}
	else if (value >= 32) {
		return 6;
	}
	else if (value >= 16) {
		return 5;
	}
	else if (value >= 8) {
		return 4;
	}
	else if (value >= 4) {
		return 3;
	}
	else if (value >= 2) {
		return 2;
	}
	else if (value == 1) {
		return 1;
	}
	else {
		return 0;
	}
}
#endif
