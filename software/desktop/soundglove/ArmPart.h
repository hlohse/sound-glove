#pragma once

#include "Vector.h"
#include "MARGRotation.h"
#include "Motion.h"
#include "Temperature.h"
#include "Calibration.h"
#include <memory>

class ArmPart {
	friend class Glove;
public:
	ArmPart();
	ArmPart(const double length);

	Vector<double> toVector() const;

	SOUNDGLOVE_API Motion rotation() const;
	SOUNDGLOVE_API Temperature temperature() const;
	SOUNDGLOVE_API Calibration margCalibration() const;

	SOUNDGLOVE_API double length() const; // [cm]

private:
	const double length_;
	std::shared_ptr<MARGRotation> marg_rotation_;
	Temperature temperature_;
	Calibration marg_calibration_;
};
