#include "CapacitiveTouch.h"
#include "gpio.h"

CapacitiveTouch& CapacitiveTouch::pin()
{
	static const Pin pin(PIN_CAPACITY_PORT, PIN_CAPACITY_PIN);
	static CapacitiveTouch instance(pin);
	return instance;
}

CapacitiveTouch::CapacitiveTouch(const Pin& pin)
:	pin_(pin)
{
	reset();
}

bool CapacitiveTouch::isPressed() const
{
	int loops = 0;
	GPIO_setAsInputPin(pin_.port, pin_.pin);
	while (GPIO_getInputPinValue(pin_.port, pin_.pin) != GPIO_INPUT_PIN_LOW
	&&	   loops < loops_touch_threshold)
	{
		loops++;
	}
	reset();
	return loops >= loops_touch_threshold;
}

void CapacitiveTouch::reset() const
{
	GPIO_setAsOutputPin(pin_.port, pin_.pin);
	GPIO_setOutputHighOnPin(pin_.port, pin_.pin);
}
