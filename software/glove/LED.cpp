#include "LED.h"
#include "gpio.h"

LED::LED(const Pin& pin, const bool is_on)
:   pin_(pin)
{
    GPIO_setAsOutputPin(pin_.port, pin_.pin);
    set(is_on);
}

LED::LED()
:   pin_(0, 0), is_on_(false)
{
}

LED::~LED()
{
}

bool LED::isOn() const
{
    return is_on_;
}

void LED::set(const bool is_on)
{
    is_on_ = is_on;
    isOn() ? GPIO_setOutputHighOnPin(pin_.port, pin_.pin) :
             GPIO_setOutputLowOnPin(pin_.port, pin_.pin);
}

void LED::setOn()
{
    set(true);
}

void LED::setOff()
{
    set(false);
}

void LED::toggle()
{
    isOn() ? setOff() : setOn();
}
