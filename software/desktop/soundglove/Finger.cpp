#include "Finger.h"

const double Finger::flex_per_raw = 100.0 / ((double)(raw_flex_closed - raw_flex_open));

Finger::Finger()
:	Finger(0)
{
}

Finger::Finger(const double flex)
:	flex_(flex)
{
}

double Finger::flex() const
{
	return flex_;
}

void Finger::update(const int16_t raw_flex)
{
	flex_ = (raw_flex - raw_flex_open) * flex_per_raw;
}
