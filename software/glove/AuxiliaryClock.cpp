#include "AuxiliaryClock.h"
#include "Pins.h"
#include "ucs.h"
#include "gpio.h"

AuxiliaryClock& AuxiliaryClock::instance()
{
    static AuxiliaryClock instance;
    return instance;
}

AuxiliaryClock::AuxiliaryClock()
{
	UCS_clockSignalInit(UCS_ACLK, UCS_XT1CLK_SELECT, UCS_CLOCK_DIVIDER_1);
}

uint32_t AuxiliaryClock::frequency() const
{
    return UCS_getACLK();
}

void AuxiliaryClock::driveOut()
{
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P11, GPIO_PIN0);
}
