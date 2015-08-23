#include "ClockGuard.h"
#include "MasterClock.h"
#include "SubsystemClock.h"
#include "AuxiliaryClock.h"
#include "driverlib.h"
#include <msp430.h>

ClockGuard::ClockGuard()
{
    WDT_A_hold(WDT_A_BASE);
    MasterClock::instance();
    SubsystemClock::instance().driveOut();
    AuxiliaryClock::instance().driveOut();
}
