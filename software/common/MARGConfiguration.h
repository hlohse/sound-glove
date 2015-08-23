#ifndef MARGCONFIGURATION_H_
#define MARGCONFIGURATION_H_

#include "LSM9DS0.h"
#include "Export.h"
#include <stdint.h>

struct MARGConfiguration {
	static const int size = 4 + 7;

	uint32_t frequency;
	uint8_t g_scale;
	uint8_t a_scale;
	uint8_t m_scale;
	uint8_t g_odr_cutoff;
	uint8_t a_odr;
	uint8_t a_abf;
	uint8_t m_odr;
	uint8_t m_res;

	SOUNDGLOVE_API MARGConfiguration();

	double gScaleMdps() const;
	double aScaleMg() const;
	double mScaleMgauss() const;
};

#endif /* MARGCONFIGURATION_H_ */
