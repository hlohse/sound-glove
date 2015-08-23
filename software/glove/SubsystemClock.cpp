#include "ucs.h"
#include "SubsystemClock.h"
#include "gpio.h"

SubsystemClock& SubsystemClock::instance()
{
    static SubsystemClock instance;
    return instance;
}

SubsystemClock::SubsystemClock()
{
    switch (divider) {
    case 4:  UCS_clockSignalInit(UCS_SMCLK, UCS_DCOCLKDIV_SELECT, UCS_CLOCK_DIVIDER_2);  break;
    case 8:  UCS_clockSignalInit(UCS_SMCLK, UCS_DCOCLKDIV_SELECT, UCS_CLOCK_DIVIDER_4);  break;
    case 16: UCS_clockSignalInit(UCS_SMCLK, UCS_DCOCLKDIV_SELECT, UCS_CLOCK_DIVIDER_8);  break;
    case 32: UCS_clockSignalInit(UCS_SMCLK, UCS_DCOCLKDIV_SELECT, UCS_CLOCK_DIVIDER_16); break;
    case 64: UCS_clockSignalInit(UCS_SMCLK, UCS_DCOCLKDIV_SELECT, UCS_CLOCK_DIVIDER_32); break;
    default: UCS_clockSignalInit(UCS_SMCLK, UCS_DCOCLKDIV_SELECT, UCS_CLOCK_DIVIDER_1);  break;
    }

    UCS_SMCLKOn();
}

uint32_t SubsystemClock::frequency() const
{
    return UCS_getSMCLK();
}

void SubsystemClock::driveOut()
{
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P11, GPIO_PIN2);
}
