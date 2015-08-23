#include "MARG.h"
#include "LSM9DS0.h"
#include "System.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

const float MARG::sensitivity_per_celsius_g = ((float)LSM9DS0_TEMPERATURE_SENSITIVITY_G) / MARG::temperature_range / 100;
const float MARG::sensitivity_per_celsius_a = ((float)LSM9DS0_TEMPERATURE_SENSITIVITY_A) / MARG::temperature_range / 100;
const float MARG::sensitivity_per_celsius_m = ((float)LSM9DS0_TEMPERATURE_SENSITIVITY_M) / MARG::temperature_range / 100;

MARG::MARG(const MARGConfiguration& configuration,
	     const Readout::MARG& readout,
		 const Temperature& temperature_difference,
		 const Calibration& calibration)
{
#ifdef USE_TEMPERATURE_COMPENSATION
	const int16_t temperature_offset[2] = {
		(int16_t)((temperature_difference.degreesCelsius() * LSM9DS0_OFFSET_PER_CELSIUS_G) / configuration.gScaleMdps()),
		(int16_t)((temperature_difference.degreesCelsius() * LSM9DS0_OFFSET_PER_CELSIUS_A) / configuration.aScaleMg())
	};
	const float temperature_scale[2] = {
		(float)temperature_difference.degreesCelsius() * sensitivity_per_celsius_g,
		(float)temperature_difference.degreesCelsius() * sensitivity_per_celsius_a
	};
#else
	const int16_t temperature_offset[2] = { 0, 0 };
	const float temperature_scale[2] = { 1, 1 };
#endif

	const Vector<int16_t> raw_offset_adjusted[2] = {
		readout.g - calibration.g_offset - temperature_offset[0],
		readout.a - calibration.a_offset - temperature_offset[1]
	};
	const Vector<float> scale_adjusted[2] = {
		calibration.g_scale * (1 + temperature_scale[0]),
		calibration.a_scale * (1 + temperature_scale[1])
	};

	g_ = toVector(raw_offset_adjusted[0] * scale_adjusted[0], configuration.gScaleMdps() / 1000 * M_PI / 180);
	a_ = toVector(raw_offset_adjusted[1] * scale_adjusted[1], configuration.aScaleMg() / 1000);
	m_ = toVector(readout.m * calibration.m_rotation - calibration.m_offset, configuration.mScaleMgauss() / 1000);
	
	if (calibration.hasInvertedPlacement()) {
		a_.setX(-a_.x());
		a_.setY(-a_.y());
	}
	if (!calibration.hasInvertedPlacement()) {
		g_.setX(-g_.x());
		g_.setY(-g_.y());
	}

	g_.setX(-g_.x());
	g_.setY(-g_.y());
	g_.setZ(-g_.z());
}

Vector<double> MARG::toVector(const Vector<int16_t>& readout_vector, const double factor) const
{
	return { readout_vector.x() * factor, readout_vector.y() * factor, readout_vector.z() * factor };
}

Vector<double> MARG::g() const { return g_; }
Vector<double> MARG::a() const { return a_; }
Vector<double> MARG::m() const { return m_; }
