#include <msp430.h>
#include <Timer.h>
#include "timer_a.h"

uint16_t Timer::timer_overflows;
float    Timer::duration;

void Timer::start()
{
	timer_overflows = 0;
	duration = 0;
	Timer_A_initUpModeParam params = {
		TIMER_A_CLOCKSOURCE_ACLK,
		TIMER_A_CLOCKSOURCE_DIVIDER_1,
		0xFFFF,
		TIMER_A_TAIE_INTERRUPT_ENABLE,
		TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
		TIMER_A_SKIP_CLEAR,
		true
	};
	Timer_A_initUpMode(TIMER_A0_BASE, &params);
}

void Timer::stop()
{
	const uint32_t ticks = ((uint32_t) timer_overflows) * 0xFFFF + TA0R;
    duration = (((float)ticks) / 32768) * 1000;
    Timer_A_stop(TIMER_A0_BASE);
    Timer_A_clear(TIMER_A0_BASE);
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer0Interrupt()
{
   ++Timer::timer_overflows;
}
