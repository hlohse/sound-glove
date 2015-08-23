#include "Serializer.h"
#include "Platform.h"
#include <string>
#include <string.h>
#ifdef MSP430
#include "Error.h"
#endif

Serializer::Serializer(const Serialization& serialization)
:	data_(serialization.data()),
	current_data_(serialization.data().get()),
	bit_offset_(0),
	data_end_(serialization.data().get() + serialization.size())
{
}

void Serializer::serialize(const uint8_t value, const int bits)
{
#ifdef MSP430
	if (bits <= 0 || bits > 8) {
    	Error::instance().set(Error::TYPE_SERIALIZER_BIT_EXCEED, bits);
	}
#endif
	if (bit_offset_ == 0) {
		if (bits == 8) {
			*current_data_ = value;
			current_data_++;
		}
		else {
			*current_data_ = value << (8 - bits);
			bit_offset_ += bits;
		}
	}
	else {
		// Append LSBs to current data_ byte, store MSBs to next data_ byte's front
		const int remaining_bits = 8 - bit_offset_;
		const int append_bits = bits > remaining_bits ? remaining_bits : bits;
		const int next_bits = bits > remaining_bits ? bits - remaining_bits : 0;
		const int append_mask = (1 << append_bits) - 1;
		const int next_mask = (1 << next_bits) - 1;
		const uint8_t next = (value >> append_bits) & next_mask;
		const uint8_t append = value & append_mask;

		*current_data_ |= append << (remaining_bits - append_bits);
		if (append_bits == remaining_bits) {
			current_data_++;
			if (next_bits > 0) {
				*current_data_ = next << (8 - next_bits);
				bit_offset_ = next_bits;
			}
			else {
				bit_offset_ = 0;
			}
		}
		else {
			bit_offset_ += append_bits;
		}
	}
    checkError();
}

void Serializer::serialize(uint8_t* buffer, const int length)
{
	if (bit_offset_ == 0) {
		memcpy(current_data_, buffer, length);
		current_data_ += length;
	}
	else {
		for (int i = 0; i < length; ++i) {
			serialize(buffer[i]);
		}
	}
    checkError();
}

int Serializer::size() const
{
	const int start = (int)data_.get();
	const int current = (int)current_data_;
	if (bit_offset_ == 0) {
		return current - start;
	}
	else {
		return current - start + 1;
	}
}

void Serializer::checkError()
{
#ifdef MSP430
	const int current = ((int)current_data_);
	const int end = ((int)data_end_);
    if (current > end || current == end && bit_offset_ > 0) {
    	Error::instance().set(Error::TYPE_SERIALIZER_EXCEED, current - end);
    }
#endif
}
