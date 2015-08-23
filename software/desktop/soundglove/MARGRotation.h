#pragma once

#include "MARG.h"
#include "Motion.h"
#include "Quaternion.h"

class MARGRotation {
	friend class Glove;
public:
	MARGRotation();

	Motion motion() const;

protected:
	Motion motion_;
	Quaternion quaternion_;
	EulerAngle calibration_;

	virtual void updateQuaternion(const MARG& marg, const double period_s) = 0;
	void updateMotion(const MARG& marg, const double period_s);

private:
	void update(const MARG& marg, const double period_s);
	void calibrate();
};
