#ifndef AUXILIARYCLOCK_H_
#define AUXILIARYCLOCK_H_

#include "IClock.h"
#include <stdint.h>

class AuxiliaryClock : public IClock {
public:
    static AuxiliaryClock& instance();

    // IClock
    virtual uint32_t frequency() const;
    virtual void driveOut();

private:
    AuxiliaryClock();

    // Disallow copies; (do not implement!)
    AuxiliaryClock(const AuxiliaryClock&);
    void operator=(const AuxiliaryClock&);
};

#endif /* AUXILIARYCLOCK_H_ */
