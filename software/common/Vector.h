#pragma once

#include "Export.h"
#include "Platform.h"
#include "Matrix.h"
#ifdef PLATFORM_DESKTOP
#include "../desktop/soundglove/EulerAngle.h"
#include <cmath>
#endif

/* Ignore "warning C4351: new behaviour: elements of array 'name' will be
* default initialized".
*/
#ifdef OS_WINDOWS
#pragma warning( push )
#pragma warning( disable : 4351 )
#endif

template <typename T>
struct Vector {
T values[3];

SOUNDGLOVE_API Vector()
:	values()
{
}

SOUNDGLOVE_API Vector(const T x,
					  const T y,
					  const T z)
:	values()
{
	setX(x);
	setY(y);
	setZ(z);
}

SOUNDGLOVE_API Vector(const Vector& other)
:	values()
{
	setX(other.x());
	setY(other.y());
	setZ(other.z());
}

#ifdef PLATFORM_DESKTOP
Vector(const EulerAngle& euler_angle)
:	values()
{
	values[0] = euler_angle.roll;
	values[1] = euler_angle.pitch;
	values[2] = euler_angle.yaw;
}
#endif

T x() const
{
	return values[0];
}

T y() const
{
	return values[1];
}

T z() const
{
	return values[2];
}

void setX(const T x)
{
	values[0] = x;
}

void setY(const T y)
{
	values[1] = y;
}

void setZ(const T z)
{
	values[2] = z;
}

bool isValid() const
{
	return !(x() == 0 && y() == 0 && z() == 0);
}

#ifdef PLATFORM_DESKTOP
double length() const
{
	return sqrt(x()*x() + y()*y() + z()*z());
}
#endif

void normalize()
{
	(*this) = normalized();
}

Vector normalized() const
{
	return length() != 0 ?
		Vector(x() / (T)length(), y() / (T)length(), z() / (T)length()) :
		Vector(0, 0, 0);
}

Vector cross(const Vector& other) const
{
	return {
		y() * other.z() - z() * other.y(),
		z() * other.x() - x() * other.z(),
		x() * other.y() - y() * other.x()
	};
}

void operator=(const Vector& other)
{
	setX(other.x());
	setY(other.y());
	setZ(other.z());
}

Vector operator+(const Vector& other) const
{
	return Vector(x() + other.x(), y() + other.y(), z() + other.z());
}

Vector operator+(const T value) const
{
	return Vector(x() + value, y() + value, z() + value);
}

Vector operator-(const Vector& other) const
{
	return Vector(x() - other.x(), y() - other.y(), z() - other.z());
}

Vector operator-(const T value) const
{
	return Vector(x() - value, y() - value, z() - value);
}

Vector operator*(const T value) const
{
	return Vector(x() * value, y() * value, z() * value);
}

Vector operator/(const T value) const
{
	return Vector(x() / value, y() / value, z() / value);
}

void operator+=(const Vector& other)
{
	*this = *this + other;
}

void operator-=(const Vector& other)
{
	*this = *this - other;
}

void operator*=(const T value)
{
	*this = *this * value;
}

void operator/=(const T value)
{
	*this = *this / value;
}

Vector operator*(const Vector<float>& other) const
{
	return Vector(
		(T)(((float)x()) * other.x()),
		(T)(((float)y()) * other.y()),
		(T)(((float)z()) * other.z())
	);
}

Vector operator*(const Matrix& matrix) const
{
	return Vector(
		(T)(matrix.get(0, 0) * (float)x() + matrix.get(0, 1) * (float)y() + matrix.get(0, 2) * (float)z()),
		(T)(matrix.get(1, 0) * (float)x() + matrix.get(1, 1) * (float)y() + matrix.get(1, 2) * (float)z()),
		(T)(matrix.get(2, 0) * (float)x() + matrix.get(2, 1) * (float)y() + matrix.get(2, 2) * (float)z())
	);
}

};
