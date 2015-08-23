#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include "Error.h"
#include <stdint.h>

template <typename T, uint16_t capacity_>
class RingBuffer {
public:

RingBuffer()
:	data_(),
	first_(data_),
	end_(data_)
{
}

T& peek()
{
	if (empty()) {
		Error::instance().set(Error::TYPE_RING_BUFFER_PEEK_EMPTY, Error::no_info);
	}
	return *first_;
}

void push(const T value)
{
	if (full()) {
		Error::instance().set(Error::TYPE_RING_BUFFER_PUSH_FULL, Error::no_info);
	}
	else {
		*end_ = value;
		end_ = end_ == last() ? data_ : end_ + 1;
	}
}

T pop()
{
	const T null;
	if (empty()) {
		Error::instance().set(Error::TYPE_RING_BUFFER_POP_EMPTY, Error::no_info);
		return null;
	}
	const T value = *first_;
	*first_ = null;
	first_ = first_ == last() ? data_ : first_ + 1;
	return value;
}

bool contains(const T value) const
{
	T* current = first_;

	if (empty()) {
		return false;
	}

	while (current != end_) {
		if (*current == value) {
			return true;
		}
		current = current == last() ? current : first_ + 1;
	}

	return false;
}

bool empty() const
{
	return size() == 0;
}

bool full() const
{
	return size() == capacity();
}

uint16_t size() const
{
	if (first_ == end_) {
		return 0;
	}
	else if (first_ < end_) {
		return ((uint16_t)end_) - ((uint16_t)first_);
	}
	else {
		const uint16_t front = ((uint16_t)end_) - ((uint16_t)data_);
		const uint16_t back = ((uint16_t)last()) - ((uint16_t)first_);
		return front + back;
	}
}

void clear()
{
	first_ = data_;
	end_ = data_;
}

uint16_t capacity() const
{
	return capacity_;
}

private:
	T data_[capacity_ + 1];
	T* first_;
	T* end_;

const T* last() const
{
	return data_ + capacity();
}

};

#endif /* RINGBUFFER_H_ */
