#include "System.h"
#include "Platform.h"
#include "Pins.h"
#include "ClockGuard.h"
#include "Bluetooth/Guard.h"
#include "Context.h"
#include "Bluetooth/SerialPort.h"
#include "Scheduler.h"
#include "LEDBar.h"
#include "HandshakeCommand.h"
#include "ADCGuard.h"
#include "SendBatteryChargeCommand.h"
#include "Battery.h"
#include "TemperatureCommand.h"
#include "VibrationMotor.h"

static const Pin led_bar_pins[4] = {
	Pin(PIN_LED_RIGHT_PORT,        PIN_LED_RIGHT_PIN),
    Pin(PIN_LED_MIDDLE_RIGHT_PORT, PIN_LED_MIDDLE_RIGHT_PIN),
    Pin(PIN_LED_MIDDLE_LEFT_PORT,  PIN_LED_MIDDLE_LEFT_PIN),
    Pin(PIN_LED_LEFT_PORT,         PIN_LED_LEFT_PIN)
};
static LEDBar<4> led_bar(led_bar_pins);
static Context context = {
	&Bluetooth::SerialPort::instance(),
	SharedPointer<Readout>(new Readout())
};
static int counter = 0;

void led(void* parameters)
{
    counter = counter == 15 ? 0 : counter + 1;
    led_bar.displayBinary(counter);
}

#ifndef SYSTEM_BENCHMARK_BLUETOOTH

void loop(void* parameters)
{
	if (context.serial_port->isOpen()) {
		Command::received(context)->execute(context);
		context.serial_port->progress();
	}
    LPM0;
}

void battery_charge(void* parameters)
{
	if (context.serial_port->isOpen()) {
		SendBatteryChargeCommand command;
		command.battery_charge = Battery::instance().charge();
		command.is_used = Battery::instance().isUsed();
		command.send(context);
	}
}

void temperature(void* parameters)
{
	if (context.serial_port->isOpen()) {
		TemperatureCommand command;
		command.execute(context);
		command.send(context);
	}
}

void main()
{
    ClockGuard clk_guard;
    ADCGuard adc_guard;
    Bluetooth::Guard bt_guard(context);
    VibrationMotor::instance().off();
    Scheduler::instance().add(loop, NULL, 1);
    Scheduler::instance().add(led, NULL, 1000);
    Scheduler::instance().add(battery_charge, NULL, 1000);
    Scheduler::instance().add(temperature, NULL, 1000);
    Scheduler::instance().run();
}

#else

// First benchmark latency, then bandwidth
void loop(void* parameters)
{
	if (context.serial_port->isOpen()) {
		SharedPointer<Command> received_command = Command::received(context);
		if (received_command->type() == Command::type_handshake) {
			HandshakeCommand command;
			command.send(context);
		}
		context.serial_port->progress();
	}
	LPM0;
}

void main()
{
    ClockGuard clk_guard;
    Bluetooth::Guard bt_guard(context);
    Scheduler::instance().add(loop, NULL, 1);
    Scheduler::instance().add(led, NULL, 1000);
    Scheduler::instance().run();
}

#endif /* !SYSTEM_BENCHMARK_BLUETOOTH */

#pragma vector=SYSNMI_VECTOR
__interrupt void ignore_isr1()
{
	if (SYSSNIV == 0x0A) { // Vacant memory access
		__no_operation();
	}
}

#pragma vector=UNMI_VECTOR
__interrupt void ignore_isr2()
{
	if (SYSUNIV == 0x04) { // Oscillator Fault
		__no_operation();
	}
}

#pragma vector=RESET_VECTOR
__interrupt void ignore_isr0()
{
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void ignore_isr3()
{
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void ignore_isr4()
{
}

#pragma vector=WDT_VECTOR
__interrupt void ignore_isr5()
{
}

#pragma vector=USCI_A0_VECTOR
__interrupt void ignore_isr6()
{
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void ignore_isr8()
{
}

#pragma vector=ADC12_VECTOR
__interrupt void ignore_isr9()
{
}

#pragma vector=USCI_B2_VECTOR
__interrupt void ignore_isr10()
{
}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void ignore_isr12()
{
}

#pragma vector=USCI_A1_VECTOR
__interrupt void ignore_isr13()
{
}

#pragma vector=USCI_A3_VECTOR
__interrupt void ignore_isr15()
{
}

#pragma vector=PORT2_VECTOR
__interrupt void ignore_isr17()
{
}

#pragma vector=RTC_VECTOR
__interrupt void ignore_isr18()
{
}
