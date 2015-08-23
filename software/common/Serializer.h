#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include "SharedPointer.h"
#include "Serialization.h"

class Serializer {
public:
	Serializer(const Serialization& serialization);

	void serialize(const uint8_t value, const int bits = 8);
	void serialize(uint8_t* buffer, const int length);

	int size() const;

private:
    SharedPointer<uint8_t> data_;
    uint8_t* current_data_;
    int bit_offset_;
    uint8_t* data_end_;

	void checkError();
};

#endif /* SERIALIZER_H_ */
