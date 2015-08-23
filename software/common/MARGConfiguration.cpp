#include "MARGConfiguration.h"
#include "LSM9DS0.h"
#include "Platform.h"

// If changed, adjust MARG::setCalibration(const Calibration& calibration)
MARGConfiguration::MARGConfiguration()
#ifdef READOUT_100HZ_SNIFF
:	frequency(100),
#else
:	frequency(500),
#endif
	g_scale(LSM9DS0::GScale::G_SCALE_2000DPS),
	a_scale(LSM9DS0::AScale::A_SCALE_16G),
	m_scale(LSM9DS0::MScale::M_SCALE_2GS),
	g_odr_cutoff(LSM9DS0::GODRCutoff::G_ODR_760_CUTOFF_100),
	a_odr(LSM9DS0::AODR::A_ODR_1600),
	a_abf(LSM9DS0::AABF::A_ABF_50),
	m_odr(LSM9DS0::MODR::M_ODR_100),
	m_res(LSM9DS0::MRES::M_RES_HIGH)
{
}

double MARGConfiguration::gScaleMdps() const
{
	switch (g_scale) {
	case LSM9DS0::GScale::G_SCALE_245DPS: return 8.75;
	case LSM9DS0::GScale::G_SCALE_500DPS: return 17.5;
	default:					          return 70; // 2000DPS
	}
}

double MARGConfiguration::aScaleMg() const
{
	switch (a_scale) {
	case LSM9DS0::AScale::A_SCALE_2G: return 0.061;
	case LSM9DS0::AScale::A_SCALE_4G: return 0.122;
	case LSM9DS0::AScale::A_SCALE_6G: return 0.183;
	case LSM9DS0::AScale::A_SCALE_8G: return 0.244;
	default:				          return 0.732; // 16G
	}
}

double MARGConfiguration::mScaleMgauss() const
{
	switch (m_scale) {
	case LSM9DS0::MScale::M_SCALE_2GS: return 0.08;
	case LSM9DS0::MScale::M_SCALE_4GS: return 0.16;
	case LSM9DS0::MScale::M_SCALE_8GS: return 0.32;
	default:				           return 0.48; // 12GS
	}
}
