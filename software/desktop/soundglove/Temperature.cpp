#include "Temperature.h"
#include "LSM9DS0.h"

const double Temperature::celsius_offset = 21;

Temperature::Temperature()
:	raw_(0)
{
}

Temperature::Temperature(const int16_t calibration, const int16_t raw)
:	raw_(raw - calibration)
{
}

Temperature::Temperature(const Unit unit, const double degrees)
:	raw_(unit == Unit::Celsius ? asRaw(degrees) : asRaw(asDegreesCelsius(degrees)))
{
}

Temperature::Temperature(const int16_t calibration, const Unit unit, const double degrees)
	: raw_((unit == Unit::Celsius ? asRaw(degrees) : asRaw(asDegreesCelsius(degrees))) - calibration)
{
}

double Temperature::degrees(const Unit unit) const
{
	return unit == Unit::Celsius ? degreesCelsius() : degreesFahrenheit();
}

double Temperature::degreesCelsius() const
{
	return asDegreesCelsius(raw());
}

double Temperature::degreesFahrenheit() const
{
	return asDegreesFahrenheit(degreesCelsius());
}

int16_t Temperature::raw() const
{
	return raw_;
}

void Temperature::setDegrees(const Unit unit, const double degrees)
{
	unit == Unit::Celsius ? setDegreesCelsius(degrees) : setDegreesFahrenheit(degrees);
}

void Temperature::setDegreesCelsius(const double degrees_celsius)
{
	setRaw(asRaw(degrees_celsius));
}

void Temperature::setDegreesFahrenheit(const double degrees_fahrenheit)
{
	setRaw(asRaw(asDegreesCelsius(degrees_fahrenheit)));
}

void Temperature::setRaw(const int16_t raw)
{
	raw_ = raw;
}

// http://www.metric-conversions.org/de/temperatur/celsius-in-fahrenheit.htm
double Temperature::asDegreesFahrenheit(const double degrees_celsius) const
{
	return 1.8 * degrees_celsius + 32;
}

double Temperature::asDegreesCelsius(const double degrees_fahrenheit) const
{
	return (degrees_fahrenheit - 32) / 1.8;
}

// https://github.com/kriswiner/LSM9DS0/blob/master/LSM9DS0_AHRS.ino#L261
double Temperature::asDegreesCelsius(const int16_t raw) const
{
	return ((double)raw) / LSM9DS0_TEMPERATURE_LSB_PER_C;
}

int16_t Temperature::asRaw(const double degrees_celsius) const
{
	return (int16_t)(LSM9DS0_TEMPERATURE_LSB_PER_C * degrees_celsius);
}
