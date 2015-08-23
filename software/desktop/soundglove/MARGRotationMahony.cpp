#include "MARGRotationMahony.h"
#include "Vector.h"
#include "EulerAngle.h"
#include <cmath>

const double MARGRotationMahony::gain_proportional = 0.5;
const double MARGRotationMahony::gain_integral = 0.0;

MARGRotationMahony::MARGRotationMahony()
:	MARGRotation(),
	feedback_(0, 0, 0)
{
}

void MARGRotationMahony::updateQuaternion(const MARG& marg, const double period_s)
{
	Vector<double> g = marg.g();

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if (marg.a().isValid()) {
		applyFeedback(marg, g, period_s);
	}

	// Integrate rate of change of quaternion
	g *= 2 * period_s;
	quaternion_ += {
		(-quaternion_.y * g.x() - quaternion_.z * g.y() - quaternion_.w * g.z()),
		( quaternion_.x * g.x() + quaternion_.z * g.z() - quaternion_.w * g.y()),
		( quaternion_.x * g.y() - quaternion_.y * g.z() + quaternion_.w * g.x()),
		( quaternion_.x * g.z() + quaternion_.y * g.y() - quaternion_.z * g.x())
	};

	// Normalise quaternion
	quaternion_.normalize();
}

void MARGRotationMahony::applyFeedback(const MARG& marg, Vector<double>& g, const double period_s)
{
	// Use MARG algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
	if (marg.m().isValid()) {
		applyFeedbackMarg(marg, g, period_s);
	}
	else {
		applyFeedbackImu(marg, g, period_s);
	}
}

void MARGRotationMahony::applyFeedbackMarg(const MARG& marg, Vector<double>& g, const double period_s)
{
	// Normalise accelerometer measurement
	const Vector<double> a = marg.a().normalized();

	// Normalise magnetometer measurement
	const Vector<double> m = marg.m().normalized();

	// Auxiliary variables
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
	const double hx = 2 * (m.x() * (0.5 - zz - ww) + m.y() * (yz - xw) + m.z() * (yw + xz));
	const double hy = 2 * (m.x() * (yz + xw) + m.y() * (0.5 - yy - ww) + m.z() * (zw - xy));
	const double bx = sqrt(hx * hx + hy * hy);
	const double bz = 2 * (m.x() * (yw - xz) + m.y() * (zw + xy) + m.z() * (0.5 - yy - zz));

	// Estimated direction of gravity and magnetic field
	const Vector<double> gravity(
		yw - xz,
		xy + zw,
		xx - 0.5 + ww
	);
	const Vector<double> magnetic(
		bx * (0.5 - zz - ww) + bz * (yw - xz),
		bx * (yz - xw) + bz * (xy + zw),
		bx * (xz + yw) + bz * (0.5 - yy - zz)
		);

	// Error is sum of cross product between estimated direction and measured direction of field
	const Vector<double> error(
		(a.y() * gravity.z() - a.z() * gravity.y()) + (m.y() * magnetic.z() - m.z() * magnetic.y()),
		(a.z() * gravity.x() - a.x() * gravity.z()) + (m.z() * magnetic.x() - m.x() * magnetic.z()),
		(a.x() * gravity.y() - a.y() * gravity.x()) + (m.x() * magnetic.y() - m.y() * magnetic.x())
	);

	applyFeedback(marg, error, g, period_s);
}

void MARGRotationMahony::applyFeedbackImu(const MARG& marg, Vector<double>& g, const double period_s)
{
	// Normalise accelerometer measurement
	const Vector<double> a = marg.a().normalized();

	// Estimated direction of gravity and vector perpendicular to magnetic flux
	const Vector<double> magnetic_gravity(
		quaternion_.y * quaternion_.w - quaternion_.x * quaternion_.z,
		quaternion_.x * quaternion_.y + quaternion_.z * quaternion_.w,
		quaternion_.x * quaternion_.x - 0.5 + quaternion_.w * quaternion_.w
	);

	// Error is sum of cross product between estimated and measured direction of gravity
	const Vector<double> error(
		a.y() * magnetic_gravity.z() - a.z() * magnetic_gravity.y(),
		a.z() * magnetic_gravity.x() - a.x() * magnetic_gravity.z(),
		a.x() * magnetic_gravity.y() - a.y() * magnetic_gravity.x()
	);

	applyFeedback(marg, error, g, period_s);
}

void MARGRotationMahony::applyFeedback(const MARG& marg, const Vector<double>& error, Vector<double>& g, const double period_s)
{
	// Compute and apply integral feedback if enabled
	if (gain_integral > 0) {
		// integral error scaled by Ki
		feedback_ += {
			2 * gain_integral * error.x() * period_s,
			2 * gain_integral * error.y() * period_s,
			2 * gain_integral * error.z() * period_s
		};
		// apply integral feedback
		g += feedback_;
	}
	else {
		// prevent integral windup
		feedback_ = { 0, 0, 0 };
	}

	// Apply proportional feedback
	g += error * 2 * gain_proportional;
}
