#ifndef MASTERCLOCK_H_
#define MASTERCLOCK_H_

#include "IClock.h"
#include "HRDWCFG.h"

class MasterClock : public IClock {
public:
    static MasterClock& instance();

    // IClock
    virtual uint32_t frequency() const;
    virtual void driveOut();

private:
	static const uint32_t xt1_frequency = 32768;
    static const Cpu_Frequency_t mclk_frequency = BT_CPU_FREQ;

    MasterClock();
    bool isVoltageCoreLevelAdjusted(const Cpu_Frequency_t frequency) const;

    // Disallow copies; (do not implement!)
    MasterClock(const MasterClock&);
    void operator=(const MasterClock&);
};

#endif /* MASTERCLOCK_H_ */
