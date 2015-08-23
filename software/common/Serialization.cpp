#include "Serialization.h"
#include "Platform.h"
#include <stdlib.h>
#ifdef PLATFORM_DESKTOP
#include <cassert>
#else
#include "Error.h"
#endif

Serialization::Serialization()
:	data_(),
	size_(0)
{
}

Serialization::Serialization(const int max_size)
:	data_(),
	size_(max_size)
{
	if (this->size() > 0) {
		data_ = SharedPointer<uint8_t>(new uint8_t[this->size()]());
	}
}

SharedPointer<uint8_t> Serialization::data() const
{
	return data_;
}

int Serialization::size() const
{
	return size_;
}

void Serialization::setSize(const int size)
{
#ifdef PLATFORM_DESKTOP
	assert(size > 0);
	assert(size <= size_);
#else
	if (size < 0 || size > size_) {
		Error::instance().set(Error::TYPE_SERIALIZATION_SIZE_EXCEED, size - size_);
	}
#endif
	size_ = size;
}
