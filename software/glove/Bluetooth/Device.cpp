#include "Bluetooth\Device.h"
#include "Bluetooth\Stack.h"
#include "Error.h"
#include "GAPAPI.h"
#include <stdlib.h>
using namespace std;
using namespace Bluetooth;

Device& Device::instance()
{
    static Device instance;
    return instance;
}

Device::Device()
:   address_()
{
    const int status = GAP_Query_Local_BD_ADDR(Stack::instance().id(), &address_);
    if (status < 0) {
        Error::instance().set(Error::TYPE_BT_DEVICE_QUERY_BD_ADDR, status);
    }
}

Device::~Device()
{
    setPairable(false);
    setDiscoverable(false);
    setConnectable(false);
}

void Device::setConnectable(const bool state)
{
    const GAP_Connectability_Mode_t mode = state ? cmConnectableMode : cmNonConnectableMode;
    const int status = GAP_Set_Connectability_Mode(Stack::instance().id(), mode);
    if (status != 0) {
        Error::instance().set(Error::TYPE_BT_DEVICE_SET_CONN, status);
    }
}

void Device::setDiscoverable(const bool state)
{
    const GAP_Discoverability_Mode_t mode = state ? dmGeneralDiscoverableMode : dmNonDiscoverableMode;
    const int status = GAP_Set_Discoverability_Mode(Stack::instance().id(), mode, 0);
    if (status != 0) {
        Error::instance().set(Error::TYPE_BT_DEVICE_SET_DISC, status);
    }
}

void Device::setPairable(const bool state)
{
    const GAP_Pairability_Mode_t mode = state ? pmPairableMode_EnableSecureSimplePairing : pmNonPairableMode;
    const int status = GAP_Set_Pairability_Mode(Stack::instance().id(), mode);
    if (status != 0) {
        Error::instance().set(Error::TYPE_BT_DEVICE_SET_PAIR, status);
    }
}

bool Device::isConntable() const
{
    GAP_Connectability_Mode_t mode;
    const int status = GAP_Query_Connectability_Mode(Stack::instance().id(), &mode);
    if (status != 0) {
        Error::instance().set(Error::TYPE_BT_DEVICE_QUERY_CONN, status);
    }
    return mode == cmConnectableMode;
}

bool Device::isDiscoverable() const
{
    GAP_Discoverability_Mode_t mode;
    unsigned long timeout;
    const int status = GAP_Query_Discoverability_Mode(Stack::instance().id(), &mode, &timeout);
    if (status != 0) {
        Error::instance().set(Error::TYPE_BT_DEVICE_QUERY_DISC, status);
    }
    return mode == dmGeneralDiscoverableMode;
}

bool Device::isPairable() const
{
    GAP_Pairability_Mode_t mode;
    const int status = GAP_Query_Pairability_Mode(Stack::instance().id(), &mode);
    if (status != 0) {
        Error::instance().set(Error::TYPE_BT_DEVICE_QUERY_PAIR, status);
    }
    return mode == pmPairableMode_EnableSecureSimplePairing;
}

BD_ADDR_t Device::address() const
{
    return address_;
}

string Device::addressString() const
{
    char address_string[17];
    BTPS_SprintF(address_string,
                 "%02X:%02X:%02X:%02X:%02X:%02X",
                 address_.BD_ADDR5,
                 address_.BD_ADDR4,
                 address_.BD_ADDR3,
                 address_.BD_ADDR2,
                 address_.BD_ADDR1,
                 address_.BD_ADDR0);
    return string(address_string, 17);
}
