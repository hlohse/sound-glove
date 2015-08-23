#include "Readout.h"
#ifdef MSP430
#include "MARG.h"
#include "Flex.h"
#include "CapacitiveTouch.h"
#endif

Readout::Readout()
:   margs(),
	flex()
{
}

Readout::MARG Readout::handImu() const
{
	return margs[0];
}

Readout::MARG Readout::forearmImu() const
{
	return margs[1];
}

Readout::MARG Readout::upperArmImu() const
{
	return margs[2];
}

#ifdef MSP430
void Readout::capture()
{
    const uint16_t flex_value = Flex::instance().flex();
    for (int i = 0; i < 5; ++i) {
    	flex[i] = flex_value;
    }
    is_touched = CapacitiveTouch::pin().isPressed() ? 1 : 0;
}
#endif

#ifdef OS_WINDOWS
#pragma warning( pop )
#endif
