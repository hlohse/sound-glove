#include "Deserializer.h"
#include "Platform.h"
#include <string>
#include <string.h>
#ifdef MSP430
#include "Error.h"
#endif

Deserializer::Deserializer(const Serialization& serialization)
:   data_(serialization.data()),
	current_data_(serialization.data().get()),
	bit_offset_(0),
	data_end_(serialization.data().get() + serialization.size())
{
}

uint8_t Deserializer::deserialize(const int bits)
{
#ifdef MSP430
	if (bits < 0 || bits > 8) {
    	Error::instance().set(Error::TYPE_DESERIALIZER_BIT_EXCEED, bits);
	}
#endif
    uint8_t value;
    if (bit_offset_ == 0) {
    	if (bits == 8) {
			value = *current_data_;
			current_data_++;
    	}
    	else {
    		// Left-aligned
    		const int bit_mask = (1 << bits) - 1;
    		const int value_mask = bit_mask << (8 - bits);
			value = (*current_data_ & value_mask) >> (8 - bits);
			bit_offset_ += bits;
    	}
    }
    else {
    	const int remaining_bits = 8 - bit_offset_;
    	const int end_bits = bits > remaining_bits ? remaining_bits : bits;
    	const int next_bits = bits > remaining_bits ? bits - remaining_bits : 0;
    	const int end_mask = ((1 << end_bits) - 1) << (remaining_bits - end_bits);
    	const int next_mask = ((1 << next_bits) - 1) << (8 - next_bits);
    	const uint8_t lsb_part = ((*current_data_ & end_mask) >> (remaining_bits - end_bits));
    	uint8_t msb_part = 0;

    	if (end_bits == remaining_bits) {
    		current_data_++;
    		if (next_bits > 0) {
				msb_part = (*current_data_ & next_mask) >> (8 - (end_bits + next_bits));
    			bit_offset_ = next_bits;
    		}
    		else {
    			bit_offset_ = 0;
    		}
    	}
    	else {
    		bit_offset_ += end_bits;
    	}

    	value = msb_part | lsb_part;
    }
    checkError();
    return value;
}

void Deserializer::deserialize(uint8_t* buffer, const int length)
{
	if (bit_offset_ == 0) {
		memcpy(buffer, current_data_, length);
		current_data_ += length;
	}
	else {
		for (int i = 0; i < length; ++i) {
			buffer[i] = deserialize();
		}
	}
    checkError();
}

void Deserializer::checkError()
{
#ifdef MSP430
	const int current = ((int)current_data_);
	const int end = ((int)data_end_);
    if (current > end || current == end && bit_offset_ > 0) {
    	Error::instance().set(Error::TYPE_DESERIALIZER_EXCEED, current - end);
    }
#endif
}
