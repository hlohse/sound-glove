#ifndef DESERIALIZER_H_
#define DESERIALIZER_H_

#include "SharedPointer.h"
#include "Serialization.h"

class Deserializer {
public:
	Deserializer(const Serialization& serialization);

	uint8_t deserialize(const int bits = 8);
	void deserialize(uint8_t* buffer, const int length);

private:
    SharedPointer<uint8_t> data_;
    uint8_t* current_data_;
    int bit_offset_;
    uint8_t* data_end_;

	void checkError();
};

#endif /* DESERIALIZER_H_ */
