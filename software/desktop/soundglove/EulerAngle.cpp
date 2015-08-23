#include "EulerAngle.h"
#include "Quaternion.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
using namespace std;

const double EulerAngle::epsilon = 1e-10;

EulerAngle::EulerAngle()
:	roll(0.0),
	pitch(0.0),
	yaw(0.0)
{
}

EulerAngle::EulerAngle(const double roll,
					   const double pitch,
					   const double yaw)
:	roll(roll),
	pitch(pitch),
	yaw(yaw)
{
	adjustRange(this->roll);
	adjustRange(this->pitch);
	adjustRange(this->yaw);
}

// https://cs.stanford.edu/~acoates/quaternion.h @ TVector3 euler(void) const
// https://math.stackexchange.com/a/693368
EulerAngle::EulerAngle(const Quaternion& quaternion)
	: EulerAngle()
{
	const Quaternion squared = quaternion * quaternion;
	pitch = asin(-2.0 * (quaternion.y * quaternion.w - quaternion.x * quaternion.z));
	const bool gimbal_locked = (M_PI / 2 - fabs(pitch)) <= epsilon;

	if (gimbal_locked) {
		yaw = atan2(2 * quaternion.y * quaternion.z - 2 * quaternion.x * quaternion.w,
			2 * quaternion.x * quaternion.z + 2 * quaternion.y * quaternion.w);
		roll = 0.0;

		if (pitch < 0) {
			yaw = M_PI - yaw;
		}
	}
	else {
		roll = atan2(quaternion.z * quaternion.w + quaternion.x * quaternion.y,
			0.5 - (squared.y + squared.z));
		yaw = atan2(quaternion.y * quaternion.z + quaternion.x * quaternion.w,
			0.5 - (squared.z + squared.w));
	}

	roll  *= 180.0 / M_PI;
	pitch *= 180.0 / M_PI;
	yaw   *= 180.0 / M_PI;

	adjustRange(roll);
	adjustRange(pitch);
	adjustRange(yaw);
}

void EulerAngle::adjustRange(double& angle)
{
	if (abs(angle) > 360) {
		if (angle >= 0) {
			angle -= 360;
		}
		else {
			angle += 360;
		}
	}
	if (abs(angle) > 180) {
		if (angle >= 0) {
			angle = -180 + (angle - 180);
		}
		else {
			angle = 360 - abs(angle);
		}
	}

	/*
	if (angle < -360.0) {
		angle += 360;
	}
	if (angle < -180.0) {
		angle = 360 - abs(angle);
	}
	if (angle > 360.0) {
		angle -= 360;
	}
	if (angle > 180.0) {
		angle = -180 + (angle - 180);
	}
	*/
}

EulerAngle EulerAngle::operator-(const EulerAngle& other) const
{
	return { roll - other.roll, pitch - other.pitch, yaw - other.yaw };
}
