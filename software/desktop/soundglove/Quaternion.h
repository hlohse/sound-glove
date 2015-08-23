#pragma once

#include "Vector.h"
#include <stdint.h>

struct Quaternion {
	double x;
	double y;
	double z;
	double w;

	Quaternion();
	Quaternion(const double x,
		       const double y,
		       const double z,
		       const double w);
	Quaternion(const Quaternion& other);
	Quaternion(const Vector<int16_t>& acceleration,
			   const Vector<int16_t>& magnetism);

	double length() const;
	void normalize();
	Quaternion normalized() const;

	void operator=(const Quaternion& other);
	void operator+=(const Quaternion& other);
	void operator-=(const Quaternion& other);

	void operator*=(const double value);
	void operator/=(const double value);

	Quaternion operator-(const Quaternion& other) const;
	Quaternion operator*(const Quaternion& other) const;

	Quaternion operator/(const double value) const;
	Quaternion operator*(const double value) const;
};
