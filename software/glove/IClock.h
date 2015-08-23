#ifndef ICLOCK_H_
#define ICLOCK_H_

#include <stdint.h>

class IClock {
public:
    virtual uint32_t frequency() const = 0;
    virtual void driveOut() = 0;
};

#endif /* ICLOCK_H_ */
