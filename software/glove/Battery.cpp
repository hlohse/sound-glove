#include "Battery.h"
#include "Pins.h"
#include "adc12_a.h"
#include "gpio.h"

Battery& Battery::instance()
{
	static Battery instance;
	return instance;
}

Battery::Battery()
{
	ADC12_A_configureMemoryParam adc_memory_parameters = {
		ADC12_A_MEMORY_5,
		ADC12_A_INPUT_A5,
		ADC12_A_VREFPOS_AVCC,
		ADC12_A_VREFNEG_AVSS,
		ADC12_A_ENDOFSEQUENCE
	};
	ADC12_A_configureMemory(ADC12_A_BASE, &adc_memory_parameters);
	GPIO_setAsInputPinWithPullDownResistor(PIN_USB_CONNECTED_PORT, PIN_USB_CONNECTED_PIN);
}

uint16_t Battery::charge() const
{
	ADC12_A_startConversion(ADC12_A_BASE, ADC12_A_MEMORY_5, ADC12_A_SINGLECHANNEL);
	while(!ADC12_A_getInterruptStatus(ADC12_A_BASE, ADC12_A_IFG5));
	ADC12_A_clearInterrupt(ADC12_A_BASE, ADC12_A_IFG5);
	return ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_5);
}

bool Battery::isUsed() const
{
	return GPIO_getInputPinValue(PIN_USB_CONNECTED_PORT, PIN_USB_CONNECTED_PIN) == 0;
}
