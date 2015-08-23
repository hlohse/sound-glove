#include "References.h"

References::References()
:	count_(0)
{
}

bool References::any() const
{
	return count_ > 0;
}

void References::acquire()
{
	count_++;
}

void References::release()
{
	count_--;
}
