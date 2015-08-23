#ifndef CAPACITIVETOUCH_H_
#define CAPACITIVETOUCH_H_

#include "Pins.h"

class CapacitiveTouch {
public:
	static CapacitiveTouch& pin();

	bool isPressed() const;

private:
	static const int loops_touch_threshold = 5;

	const Pin pin_;

	CapacitiveTouch(const Pin& pin);

	void reset() const;
};

#endif /* CAPACITIVETOUCH_H_ */
