#include "ADCGuard.h"
#include "Error.h"
#include "Battery.h"
#include "Flex.h"
#include "adc12_a.h"

ADCGuard::ADCGuard()
{
	if (ADC12_A_init(ADC12_A_BASE,
			         ADC12_A_SAMPLEHOLDSOURCE_SC,
			         ADC12_A_CLOCKSOURCE_ADC12OSC,
					 ADC12_A_CLOCKDIVIDER_1) != STATUS_SUCCESS)
	{
		Error::instance().set(Error::TYPE_ADC_INIT, Error::no_info);
	}
	else {
		ADC12_A_enable(ADC12_A_BASE);
		ADC12_A_setupSamplingTimer(ADC12_A_BASE,
				                   ADC12_A_CYCLEHOLD_4_CYCLES,
				                   ADC12_A_CYCLEHOLD_4_CYCLES,
								   ADC12_A_MULTIPLESAMPLESDISABLE);

	}
	Battery::instance();
	Flex::instance();
}
