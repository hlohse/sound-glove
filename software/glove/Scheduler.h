#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "BKRNLAPI.h"
#include <stdlib.h>

class Scheduler {
public:
    typedef BTPS_SchedulerFunction_t function;

    static Scheduler& instance();

    void add(const function& function, void* parameters, const int period_ms);
    void remove(const function& function, void* parameters);

    // Never returns
    void run();

private:
    Scheduler();
};

#endif /* SCHEDULER_H_ */
