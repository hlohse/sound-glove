#pragma once

#include "Export.h"
#include <stdint.h>

class Temperature {
public:
	enum class Unit {
		Celsius,
		Fahrenheit
	};

	SOUNDGLOVE_API Temperature();
	SOUNDGLOVE_API Temperature(const int16_t calibration, const int16_t raw);
	SOUNDGLOVE_API Temperature(const Unit unit, const double degrees);
	SOUNDGLOVE_API Temperature(const int16_t calibration, const Unit unit, const double degrees);

	SOUNDGLOVE_API double degrees(const Unit unit) const;
	SOUNDGLOVE_API double degreesCelsius() const;
	SOUNDGLOVE_API double degreesFahrenheit() const;
	SOUNDGLOVE_API int16_t raw() const;

	SOUNDGLOVE_API void setDegrees(const Unit unit, const double degrees);
	SOUNDGLOVE_API void setDegreesCelsius(const double degrees_celsius);
	SOUNDGLOVE_API void setDegreesFahrenheit(const double degrees_fahrenheit);
	SOUNDGLOVE_API void setRaw(const int16_t raw);

private:
	static const double celsius_offset;

	int16_t raw_;

	double asDegreesFahrenheit(const double degrees_celsius) const;
	double asDegreesCelsius(const double degrees_fahrenheit) const;
	double asDegreesCelsius(const int16_t raw) const;
	int16_t asRaw(const double degrees_celsius) const;
};
