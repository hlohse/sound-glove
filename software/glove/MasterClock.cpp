#include "MasterClock.h"
#include "Pins.h"
#include "ucs.h"
#include "pmm.h"
#include "gpio.h"

MasterClock& MasterClock::instance()
{
    static MasterClock instance;
    return instance;
}

MasterClock::MasterClock()
{
	const Pin xtal32k_in(PIN_XTAL32K_IN_PORT, PIN_XTAL32K_IN_PIN);
	const Pin xtal32k_out(PIN_XTAL32K_OUT_PORT, PIN_XTAL32K_OUT_PIN);

	// Use XT1 pins for external quarz
	GPIO_setAsPeripheralModuleFunctionInputPin(xtal32k_in.port, xtal32k_in.pin);
	GPIO_setAsPeripheralModuleFunctionOutputPin(xtal32k_out.port, xtal32k_out.pin);

	// Initialize external quarz as XT1 clock
	UCS_setExternalClockSource(xt1_frequency, 0);
	UCS_LFXT1Start(UCS_XT1_DRIVE3, UCS_XCAP_3);
    UCS_clockSignalInit(UCS_FLLREF, UCS_XT1CLK_SELECT, UCS_CLOCK_DIVIDER_1);

    // Run MCLK with desired frequency from external XT1 clock
	while (!isVoltageCoreLevelAdjusted(mclk_frequency));
	UCS_initFLLSettle(mclk_frequency / 1000, mclk_frequency / UCS_REFOCLK_FREQUENCY);
}

bool MasterClock::isVoltageCoreLevelAdjusted(const Cpu_Frequency_t frequency) const
{
    const int mhz = frequency / 1e6;
    uint8_t voltage_core_level = PMM_CORE_LEVEL_3;

    if (mhz <= 8) {
        voltage_core_level = PMM_CORE_LEVEL_0;
    }
    else if (mhz <= 12) {
        voltage_core_level = PMM_CORE_LEVEL_1;
    }
    else if (mhz <= 20) {
        voltage_core_level = PMM_CORE_LEVEL_2;
    }

    return PMM_setVCore(voltage_core_level) == STATUS_SUCCESS;
}

uint32_t MasterClock::frequency() const
{
    return UCS_getMCLK();
}

void MasterClock::driveOut()
{
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P11, GPIO_PIN1);
}
