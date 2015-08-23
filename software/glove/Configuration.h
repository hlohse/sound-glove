#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include "FlashMemory.h"
#include "MARG.h"
#include "Calibration.h"
#include "BTBTypes.h"
#include <stdint.h>

class Configuration {
public:
	static Configuration& instance();

	char* name() const;
	void setName(const char* name, const int length);
	int nameLength() const;

	BD_ADDR_t& remoteAddress() const;
	void setRemoteAddress(const BD_ADDR_t& remote_address);

	Link_Key_t& linkKey() const;
	void setLinkKey(const Link_Key_t& link_key);

	Calibration& margCalibration(const MARG::Placement marg) const;
	void setImuCalibration(const MARG::Placement marg,
					const Calibration& calibration);

private:
	static const char* defaultName();

    Configuration();

    bool isInitialized() const;
    void setInitialized() const;

    FlashMemory& margCalibrationSource(const MARG::Placement marg) const;

    // Disallow copies; (do not implement!)
    Configuration(const Configuration&);
    void operator=(const Configuration&);
};

#endif /* CONFIGURATION_H_ */
