#include "VibrationMotor.h"
#include "gpio.h"

VibrationMotor& VibrationMotor::instance()
{
	static VibrationMotor instance(Pin(PIN_MOTOR_PORT, PIN_MOTOR_PIN));
	return instance;
}

void VibrationMotor::instancePwm(void* parameters)
{
	instance().toggle();
}

VibrationMotor::VibrationMotor(const Pin& pin)
:	pin_(pin),
	is_on_(false)
{
	GPIO_setAsOutputPin(pin_.port, pin_.pin);
	off();
}

bool VibrationMotor::isOn() const
{
	return is_on_;
}

void VibrationMotor::on()
{
	GPIO_setOutputHighOnPin(pin_.port, pin_.pin);
	is_on_ = true;
}

void VibrationMotor::off()
{
	GPIO_setOutputLowOnPin(pin_.port, pin_.pin);
	is_on_ = false;
}

void VibrationMotor::toggle()
{
	isOn() ? off() : on();
}
