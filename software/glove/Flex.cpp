#include "Flex.h"
#include "adc12_a.h"

Flex& Flex::instance()
{
	static Flex instance;
	return instance;
}

Flex::Flex()
{
	ADC12_A_configureMemoryParam adc_memory_parameters = {
		ADC12_A_MEMORY_4,
		ADC12_A_INPUT_A4,
		ADC12_A_VREFPOS_AVCC,
		ADC12_A_VREFNEG_AVSS,
		ADC12_A_ENDOFSEQUENCE
	};
	ADC12_A_configureMemory(ADC12_A_BASE, &adc_memory_parameters);
}

uint16_t Flex::flex() const
{
	ADC12_A_startConversion(ADC12_A_BASE, ADC12_A_MEMORY_4, ADC12_A_SINGLECHANNEL);
	while(!ADC12_A_getInterruptStatus(ADC12_A_BASE, ADC12_A_IFG4));
	ADC12_A_clearInterrupt(ADC12_A_BASE, ADC12_A_IFG4);
	return ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_4);
}
