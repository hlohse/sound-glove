#include "MARGRotationFactory.h"
#include "EulerAngle.h"
#include "Vector.h"

MARGRotation::MARGRotation()
:	motion_(),
	quaternion_(1, 0, 0, 0),
	calibration_()
{
}

void MARGRotation::update(const MARG& marg, const double period_s)
{
	updateQuaternion(marg, period_s);
	updateMotion(marg, period_s);
}

void MARGRotation::updateMotion(const MARG& marg, const double period_s)
{
	const EulerAngle euler_angle = EulerAngle(quaternion_) - calibration_;
	const Vector<double> position(euler_angle);
	const Vector<double> speed = marg.g();
	const Vector<double> acceleration = (speed - motion().speed()) / period_s;
	motion_ = { position, speed, acceleration };
}

Motion MARGRotation::motion() const
{
	return motion_;
}

void MARGRotation::calibrate()
{
	calibration_ = { quaternion_ };
}
