#ifndef SERIALIZATION_H_
#define SERIALIZATION_H_

#include "SharedPointer.h"
#include <stdint.h>

class Serialization {
public:
	Serialization();
	Serialization(const int max_size);

	SharedPointer<uint8_t> data() const;
	int size() const;
	void setSize(const int size);

private:
	SharedPointer<uint8_t> data_;
    int size_;
};

#endif /* SERIALIZATION_H_ */
