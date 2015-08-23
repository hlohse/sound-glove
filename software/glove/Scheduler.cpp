#include "Scheduler.h"
#include "Error.h"
#include <stdlib.h>

Scheduler& Scheduler::instance()
{
    static Scheduler instance;
    return instance;
}

Scheduler::Scheduler()
{
}

void Scheduler::add(const function& function, void* parameters, const int period_ms)
{
    const Boolean_t status = BTPS_AddFunctionToScheduler(function,
    													 parameters,
                                                         (unsigned int) period_ms);
    if (status == FALSE) {
        Error::instance().set(Error::TYPE_SCHEDULER_SCHEDULE, Error::no_info);
    }
}

void Scheduler::remove(const function& function, void* parameters)
{
	BTPS_DeleteFunctionFromScheduler(function, parameters);
}

void Scheduler::run()
{
    for (;;) {
        BTPS_ExecuteScheduler();
    }
}
