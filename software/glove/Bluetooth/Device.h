#ifndef BLUETOOTH_DEVICE_H_
#define BLUETOOTH_DEVICE_H_

#include <string>
#include "BTBTypes.h"

namespace Bluetooth {

class Device {
public:
    static Device& instance();
    ~Device();

    // Set Error
    void setConnectable(const bool state);
    void setDiscoverable(const bool state);
    void setPairable(const bool state);

    // Set Error
    bool isConntable() const;
    bool isDiscoverable() const;
    bool isPairable() const;

    BD_ADDR_t address() const;
    std::string addressString() const;

private:
    BD_ADDR_t address_;

    // Sets Error
    Device();

    // Disallow copies; (do not implement!)
    Device(const Device&);
    void operator=(const Device&);
};

}

#endif /* BLUETOOTH_DEVICE_H_ */
