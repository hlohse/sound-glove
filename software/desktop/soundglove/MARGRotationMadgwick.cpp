#include "MARGRotationMadgwick.h"
#include "EulerAngle.h"
#include <cmath>

MARGRotationMadgwick::MARGRotationMadgwick()
:	MARGRotation(),
	beta_(2),
	zeta_(0),
	g_drift_()
{
}

void MARGRotationMadgwick::updateQuaternion(const MARG& marg, const double period_s)
{
	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if (marg.a().isValid()) {
		quaternion_ += getFeedback(marg, period_s) * period_s;
	}
	else {
		// Rate of change of quaternion from gyroscope
		const Quaternion g_change(
			0.5 * (-quaternion_.y * marg.g().x() - quaternion_.z * marg.g().y() - quaternion_.w * marg.g().z()),
			0.5 * ( quaternion_.x * marg.g().x() + quaternion_.z * marg.g().z() - quaternion_.w * marg.g().y()),
			0.5 * ( quaternion_.x * marg.g().y() - quaternion_.y * marg.g().z() + quaternion_.w * marg.g().x()),
			0.5 * ( quaternion_.x * marg.g().z() + quaternion_.y * marg.g().y() - quaternion_.z * marg.g().x()));
		// Integrate rate of change of quaternion to yield quaternion
		quaternion_ += g_change * period_s;
	}
	quaternion_.normalize();
}

Quaternion MARGRotationMadgwick::getFeedback(const MARG& marg, const double period_s)
{
	// Use MARG algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
	return marg.m().isValid() ? getFeedbackMarg(marg, period_s) : getFeedbackImu(marg, period_s);
}

Quaternion MARGRotationMadgwick::getFeedbackMarg(const MARG& marg, const double period_s)
{
	// Normalise accelerometer measurement
	const Vector<double> a = marg.a().normalized();

	// Normalise magnetometer measurement
	const Vector<double> m = marg.m().normalized();

	// Auxiliary variables to avoid repeated arithmetic
	const double _2xmx = 2.0f * quaternion_.x * m.x();
	const double _2xmy = 2.0f * quaternion_.x * m.y();
	const double _2xmz = 2.0f * quaternion_.x * m.z();
	const double _2ymx = 2.0f * quaternion_.y * m.x();
	const double _2x = 2.0f * quaternion_.x;
	const double _2y = 2.0f * quaternion_.y;
	const double _2z = 2.0f * quaternion_.z;
	const double _2w = 2.0f * quaternion_.w;
	const double _2xz = 2.0f * quaternion_.x * quaternion_.z;
	const double _2zw = 2.0f * quaternion_.z * quaternion_.w;
	const double xx = quaternion_.x * quaternion_.x;
	const double xy = quaternion_.x * quaternion_.y;
	const double xz = quaternion_.x * quaternion_.z;
	const double xw = quaternion_.x * quaternion_.w;
	const double yy = quaternion_.y * quaternion_.y;
	const double yz = quaternion_.y * quaternion_.z;
	const double yw = quaternion_.y * quaternion_.w;
	const double zz = quaternion_.z * quaternion_.z;
	const double zw = quaternion_.z * quaternion_.w;
	const double ww = quaternion_.w * quaternion_.w;

	// Reference direction of Earth's magnetic field
	const double hx = m.x() * xx - _2xmy * quaternion_.w + _2xmz * quaternion_.z
		+ m.x() * yy + _2y * m.y() * quaternion_.z + _2y * m.z() * quaternion_.w
		- m.x() * zz - m.x() * ww;
	const double hy = _2xmx * quaternion_.w + m.y() * xx - _2xmz * quaternion_.y
		+ _2ymx * quaternion_.z - m.y() * yy + m.y() * zz + _2z * m.z() * quaternion_.w
		- m.y() * ww;
	const double _2bx = sqrt(hx * hx + hy * hy);
	const double _2bz = -_2xmx * quaternion_.z + _2xmy * quaternion_.y + m.z() * xx +
		_2ymx * quaternion_.w - m.z() * yy + _2z * m.y() * quaternion_.w
		- m.z() * zz + m.z() * ww;
	const double _4bx = 2.0f * _2bx;
	const double _4bz = 2.0f * _2bz;

	// Gradient decent algorithm corrective step
	const Quaternion feedback = Quaternion(
		-_2z * (2.0f * yw - _2xz - a.x()) + _2y * (2.0f * xy + _2zw - a.y())
		- _2bz * quaternion_.z * (_2bx * (0.5f - zz - ww) +
		_2bz * (yw - xz) - m.x()) + (-_2bx * quaternion_.w + _2bz * quaternion_.y)
		* (_2bx * (yz - xw) + _2bz * (xy + zw) - m.y())
		+ _2bx * quaternion_.z * (_2bx * (xz + yw) + _2bz * (0.5f - yy - zz) - m.z()),

		_2w * (2.0f * yw - _2xz - a.x()) + _2x * (2.0f * xy + _2zw - a.y())
		- 4.0f * quaternion_.y * (1 - 2.0f * yy - 2.0f * zz - a.z())
		+ _2bz * quaternion_.w * (_2bx * (0.5f - zz - ww) + _2bz * (yw - xz) - m.x())
		+ (_2bx * quaternion_.z + _2bz * quaternion_.x) * (_2bx * (yz - xw)
		+ _2bz * (xy + zw) - m.y()) + (_2bx * quaternion_.w - _4bz * quaternion_.y)
		* (_2bx * (xz + yw) + _2bz * (0.5f - yy - zz) - m.z()),

		-_2x * (2.0f * yw - _2xz - a.x()) + _2w * (2.0f * xy + _2zw - a.y())
		- 4.0f * quaternion_.z * (1 - 2.0f * yy - 2.0f * zz - a.z())
		+ (-_4bx * quaternion_.z - _2bz * quaternion_.x) * (_2bx * (0.5f - zz - ww)
			+ _2bz * (yw - xz) - m.x()) + (_2bx * quaternion_.y + _2bz * quaternion_.w)
		* (_2bx * (yz - xw) + _2bz * (xy + zw) - m.y()) + (_2bx * quaternion_.x
		- _4bz * quaternion_.z) * (_2bx * (xz + yw) + _2bz * (0.5f - yy - zz) - m.z()),

		_2y * (2.0f * yw - _2xz - a.x()) + _2z * (2.0f * xy + _2zw - a.y())
		+ (-_4bx * quaternion_.w + _2bz * quaternion_.y) * (_2bx * (0.5f - zz - ww)
		+ _2bz * (yw - xz) - m.x()) + (-_2bx * quaternion_.x + _2bz * quaternion_.z)
		* (_2bx * (yz - xw) + _2bz * (xy + zw) - m.y()) + _2bx * quaternion_.y
		* (_2bx * (xz + yw) + _2bz * (0.5f - yy - zz) - m.z())
	).normalized(); // normalise step magnitude

	// compute gyro drift bias
	// https://github.com/ccny-ros-pkg/imu_tools/blob/indigo/imu_filter_madgwick/src/imu_filter.cpp
	const Vector<double> g_error = {
		_2x * feedback.y - _2y * feedback.x - _2z * feedback.w + _2w * feedback.z,
		_2x * feedback.z + _2y * feedback.w - _2z * feedback.x - _2w * feedback.y,
		_2x * feedback.w - _2y * feedback.z + _2z * feedback.y - _2w * feedback.x
	};

	g_drift_ += g_error * period_s * zeta();
	const Vector<double> g_corrected = marg.g() - g_drift_;

	// Rate of change of quaternion from gyroscope
	const Quaternion g_change(
		0.5 * (-quaternion_.y * g_corrected.x() - quaternion_.z * g_corrected.y() - quaternion_.w * g_corrected.z()),
		0.5 * ( quaternion_.x * g_corrected.x() + quaternion_.z * g_corrected.z() - quaternion_.w * g_corrected.y()),
		0.5 * ( quaternion_.x * g_corrected.y() - quaternion_.y * g_corrected.z() + quaternion_.w * g_corrected.x()),
		0.5 * ( quaternion_.x * g_corrected.z() + quaternion_.y * g_corrected.y() - quaternion_.z * g_corrected.x()));

	// Apply feedback step
	return g_change - (feedback * beta());
}

Quaternion MARGRotationMadgwick::getFeedbackImu(const MARG& marg, const double period_s)
{
	// Normalise accelerometer measurement
	const Vector<double> a = marg.a().normalized();

	// Auxiliary variables to avoid repeated arithmetic
	const double _2x = 2.0f * quaternion_.x;
	const double _2y = 2.0f * quaternion_.y;
	const double _2z = 2.0f * quaternion_.z;
	const double _2w = 2.0f * quaternion_.w;
	const double _4x = 4.0f * quaternion_.x;
	const double _4y = 4.0f * quaternion_.y;
	const double _4z = 4.0f * quaternion_.z;
	const double _8y = 8.0f * quaternion_.y;
	const double _8z = 8.0f * quaternion_.z;
	const double xx = quaternion_.x * quaternion_.x;
	const double yy = quaternion_.y * quaternion_.y;
	const double zz = quaternion_.z * quaternion_.z;
	const double ww = quaternion_.w * quaternion_.w;

	// Gradient decent algorithm corrective step
	const Quaternion feedback = Quaternion(
		_4x * zz + _2z * a.x() + _4x * yy - _2y * a.y(),

		_4y * ww - _2w * a.x() + 4.0f * xx * quaternion_.y
		- _2x * a.y() - _4y + _8y * yy + _8y * zz + _4y * a.z(),

		4.0f * xx * quaternion_.z + _2x * a.x() + _4z * ww
		- _2w * a.y() - _4z + _8z * yy + _8z * zz + _4z * a.z(),

		4.0f * yy * quaternion_.w - _2y * a.x()
		+ 4.0f * zz * quaternion_.w - _2z * a.y()
	).normalized(); // normalise step magnitude

	// Rate of change of quaternion from gyroscope
	const Quaternion g_change(
		0.5 * (-quaternion_.y * marg.g().x() - quaternion_.z * marg.g().y() - quaternion_.w * marg.g().z()),
		0.5 * ( quaternion_.x * marg.g().x() + quaternion_.z * marg.g().z() - quaternion_.w * marg.g().y()),
		0.5 * ( quaternion_.x * marg.g().y() - quaternion_.y * marg.g().z() + quaternion_.w * marg.g().x()),
		0.5 * ( quaternion_.x * marg.g().z() + quaternion_.y * marg.g().y() - quaternion_.z * marg.g().x()));

	// Apply feedback step
	return g_change - (feedback * beta());
}

double MARGRotationMadgwick::beta() const
{
	return beta_;
}

void MARGRotationMadgwick::setBeta(const double beta)
{
	beta_ = beta;
}

double MARGRotationMadgwick::zeta() const
{
	return zeta_;
}

void MARGRotationMadgwick::setZeta(const double zeta)
{
	zeta_ = zeta;
}
