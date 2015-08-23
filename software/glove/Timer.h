#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

// Uses TIMER0_A0 for ACLK overflow counts
class Timer {
public:
	static uint16_t timer_overflows;

	static void start();
	static void stop();
	static float duration; // [ms]
};

#endif /* TIMER_H_ */
