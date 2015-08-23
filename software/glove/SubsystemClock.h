#ifndef SUBSYSTEMCLOCK_H_
#define SUBSYSTEMCLOCK_H_

#include "IClock.h"
#include "System.h"

class SubsystemClock : public IClock {
public:
    static SubsystemClock& instance();

    // IClock
    virtual uint32_t frequency() const;
    virtual void driveOut();

private:
#ifdef SYSTEM_CLOCK_8_MHZ
    static const int divider = 1;
#endif
#ifdef SYSTEM_CLOCK_20_10_MHZ
    static const int divider = 2;
#endif
#ifdef SYSTEM_CLOCK_25_6_MHZ
    static const int divider = 4;
#endif

    SubsystemClock();

    // Disallow copies; (do not implement!)
    SubsystemClock(const SubsystemClock&);
    void operator=(const SubsystemClock&);
};

#endif /* SUBSYSTEMCLOCK_H_ */
