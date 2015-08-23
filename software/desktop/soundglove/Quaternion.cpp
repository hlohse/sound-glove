#include "Quaternion.h"
#include <array>
#include <cmath>
using namespace std;

Quaternion::Quaternion()
:	x(1.0),
	y(0.0),
	z(0.0),
	w(0.0)
{
}

Quaternion::Quaternion(const double x,
					   const double y,
					   const double z,
					   const double w)
:   x(x),
	y(y),
	z(z),
	w(w)
{
}

Quaternion::Quaternion(const Quaternion& other)
:	x(other.x),
	y(other.y),
	z(other.z),
	w(other.w)
{
}

// http://sourceforge.net/p/ce10dofahrs/code/ci/master/tree/quaternion.h#l112
Quaternion::Quaternion(const Vector<int16_t>& acceleration,
					   const Vector<int16_t>& magnetism)
{
	const Vector<double> acceleration_double(
		acceleration.x(),
		acceleration.y(),
		acceleration.z());
	const Vector<double> magnetism_double(
		magnetism.x(),
		magnetism.y(),
		magnetism.z());
	const Vector<double> down = acceleration_double.normalized();
	const Vector<double> east = down.cross(magnetism_double).normalized();
	const Vector<double> north = east.cross(down).normalized();
	const array<Vector<double>, 3> m({ { north, east, down } });
	const double tr = m[0].x() + m[1].y() + m[2].z();
	double s = 0;
	if (tr > 0) {
		s = 2 * sqrt(tr + 1);
		x = 0.25 * s;
		y = (m[2].y() - m[1].z()) / s;
		z = (m[0].z() - m[2].x()) / s;
		w = (m[1].x() - m[0].y()) / s;
	}
	else if (m[0].x() < m[1].y() && m[0].x() < m[2].z()) {
		s = 2 * sqrt(m[0].x() - m[1].y() - m[2].z() + 1);
		x = (m[2].y() - m[1].z()) / s;
		y = 0.25 * s;
		z = (m[0].y() + m[1].x()) / s;
		w = (m[0].z() + m[2].x()) / s;
	}
	else if (m[1].y() < m[2].z()) {
		s = 2 * sqrt(m[1].y() - m[0].x() - m[2].z() + 1);
		x = (m[0].z() - m[2].x()) / s;
		y = (m[0].y() + m[1].x()) / s;
		z = 0.25 * s;
		w = (m[1].z() + m[2].y()) / s;
	}
	else {
		s = 2 * sqrt(m[2].z() - m[0].x() - m[1].y() + 1);
		x = (m[1].x() - m[0].y()) / s;
		y = (m[0].z() + m[2].x()) / s;
		z = (m[1].z() + m[2].y()) / s;
		w = 0.25 * s;
	}
}

double Quaternion::length() const
{
	return sqrt(x*x + y*y + z*z + w*w);
}

void Quaternion::normalize()
{
	if (length() != 0) {
		(*this) /= length();
	}
}

Quaternion Quaternion::normalized() const
{
	Quaternion normalized = *this;
	normalized.normalize();
	return normalized;
}

void Quaternion::operator=(const Quaternion& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
}

void Quaternion::operator+=(const Quaternion& other)
{
	(*this) = { x + other.x, y + other.y, z + other.z, w + other.w };
}

void Quaternion::operator-=(const Quaternion& other)
{
	(*this) = { x - other.x, y - other.y, z - other.z, w - other.w };
}

void Quaternion::operator*=(const double value)
{
	(*this) = { x * value, y * value, z * value, w * value };
}

void Quaternion::operator/=(const double value)
{
	(*this) = { x / value, y / value, z / value, w / value };
}

Quaternion Quaternion::operator-(const Quaternion& other) const
{
	return{ x - other.x, y - other.y, z - other.z, w - other.w };
}

Quaternion Quaternion::operator*(const Quaternion& other) const
{
	return { x * other.x, y * other.y, z * other.z, w * other.w };
}

Quaternion Quaternion::operator*(const double value) const
{
	return { x * value, y * value, z * value, w * value };
}

Quaternion Quaternion::operator/(const double value) const
{
	return { x / value, y / value, z / value, w / value };
}
