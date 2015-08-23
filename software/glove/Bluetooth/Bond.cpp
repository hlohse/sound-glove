#include "Bluetooth/Bond.h"
#include "Configuration.h"
#include <string.h>
using namespace Bluetooth;

const BD_ADDR_t Bond::invalid_remote_address = {0};
const Link_Key_t Bond::invalid_link_key = {0};

Bond& Bond::instance()
{
    static Bond instance;
    return instance;
}

Bond::Bond()
{
}

bool Bond::isValid() const
{
	return !hasRemoteAddress(invalid_remote_address)
		&& !hasLinkKey(invalid_link_key);
}

BD_ADDR_t Bond::remoteAddress() const
{
    return Configuration::instance().remoteAddress();
}

void Bond::setRemoteAddress(const BD_ADDR_t& address)
{
	Configuration::instance().setRemoteAddress(address);
}

Link_Key_t Bond::linkKey() const
{
    return Configuration::instance().linkKey();
}

void Bond::setLinkKey(const Link_Key_t& link_key)
{
	Configuration::instance().setLinkKey(link_key);
}

bool Bond::hasRemoteAddress(const BD_ADDR_t& address) const
{
    return memcmp(&Configuration::instance().remoteAddress(), &address, sizeof(address)) == 0;
}

bool Bond::hasLinkKey(const Link_Key_t& link_key) const
{
    return memcmp(&Configuration::instance().linkKey(), &link_key, sizeof(link_key)) == 0;
}
