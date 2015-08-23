#ifndef SHAREDPOINTER_H_
#define SHAREDPOINTER_H_

#include "References.h"
#include <stdlib.h>

template <typename T>
class SharedPointer {
public:

SharedPointer()
:	object_((T*)NULL),
	references_((References*)NULL)
{
}

SharedPointer(T* object)
:	object_(object),
	references_(new References())
{
	references_->acquire();
}

SharedPointer(const SharedPointer<T>& other)
:	object_(other.object_),
	references_(other.references_)
{
	acquire();
}

~SharedPointer()
{
	release();
}

T& operator*()
{
	return *object_;
}

T* operator->()
{
	return object_;
}

T* get()
{
	return object_;
}

const T* get() const
{
	return object_;
}

SharedPointer<T>& operator=(const SharedPointer<T>& other)
{
	if (&other != this) {
		release();
		object_ = other.object_;
		references_ = other.references_;
		acquire();
	}
	return *this;
}

private:
	T* object_;
	References* references_;

void acquire()
{
	if (references_ != NULL) {
		references_->acquire();
	}
}

void release()
{
	if (references_ != NULL) {
		references_->release();
		if (!references_->any()) {
			delete references_;
			if (object_ != NULL) {
				delete object_;
			}
		}
	}
}

};

#endif /* SHAREDPOINTER_H_ */
