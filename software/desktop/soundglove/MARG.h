#pragma once

#include "MARGConfiguration.h"
#include "Readout.h"
#include "Temperature.h"
#include "Calibration.h"
#include "Vector.h"
#include <stdint.h>

class MARG {
public:
	MARG(const MARGConfiguration& configuration,
		const Readout::MARG& readout,
		const Temperature& temperature_difference,
		const Calibration& calibration);

	Vector<double> g() const; // [rad/s]
	Vector<double> a() const; // [g]
	Vector<double> m() const; // [gauss]

private:
	static const int temperature_min = -40;
	static const int temperature_max = 85;
	static const int temperature_range = temperature_max - temperature_min;
	static const float sensitivity_per_celsius_g;
	static const float sensitivity_per_celsius_a;
	static const float sensitivity_per_celsius_m;

	Vector<double> g_;
	Vector<double> a_;
	Vector<double> m_;

	Vector<double> toVector(const Vector<int16_t>& readout_vector, const double factor) const;
};
