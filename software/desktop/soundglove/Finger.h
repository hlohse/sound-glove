#pragma once

#include "Export.h"
#include <stdint.h>

class Finger {
	friend class Glove;
public:
	Finger();
	Finger(const double flex);

	SOUNDGLOVE_API double flex() const; // [%] (0% = stretched, 100% = closed)

private:
	static const int raw_flex_open   = 1100;
	static const int raw_flex_closed = 2000;
	static const double flex_per_raw;

	double flex_;

	void update(const int16_t raw_flex);
};
