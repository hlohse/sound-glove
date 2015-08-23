#ifndef BLUETOOTH_BOND_H_
#define BLUETOOTH_BOND_H_

#include "BTBTypes.h"

namespace Bluetooth {

class Bond {
public:
    static const BD_ADDR_t invalid_remote_address;
    static const Link_Key_t invalid_link_key;

    static Bond& instance();

    bool isValid() const;

    BD_ADDR_t remoteAddress() const;
    void setRemoteAddress(const BD_ADDR_t& address);

    Link_Key_t linkKey() const;
    void setLinkKey(const Link_Key_t& link_key);

    bool hasRemoteAddress(const BD_ADDR_t& address) const;
    bool hasLinkKey(const Link_Key_t& link_key) const;

private:
    Bond();

    // Disallow copies; (do not implement!)
    Bond(const Bond&);
    void operator=(const Bond&);
};

}

#endif /* BLUETOOTH_BOND_H_ */
