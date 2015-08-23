#include "Bluetooth/Guard.h"
#include "Bluetooth\Stack.h"
#include "Bluetooth\Device.h"
#include "Bluetooth\Bond.h"
#include "Bluetooth\GAP.h"
#include "Bluetooth\SPP.h"
#include "Bluetooth\SerialPort.h"
using namespace Bluetooth;

Guard::Guard(Context& context)
{
    Stack::instance();
	Device::instance();
	Bond::instance();
	GAP::instance().setAllowsPairing(true);
	SPP::instance(&context);
	SerialPort::instance();

	Device::instance().setPairable(true);
	Device::instance().setConnectable(true);
	Device::instance().setDiscoverable(true);
}
