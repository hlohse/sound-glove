#include "ArmPart.h"
#include "MARGRotationFactory.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

ArmPart::ArmPart()
:	ArmPart(0.0)
{
}

ArmPart::ArmPart(const double length)
:	length_(length),
	marg_rotation_(MARGRotationFactory::getImuRotation()),
	marg_calibration_()
{
}

Vector<double> ArmPart::toVector() const
{
	return { Vector<double>(
		sin(rotation().position().z() * M_PI / 180) * cos(rotation().position().y() * M_PI / 180),
		cos(rotation().position().z() * M_PI / 180) * cos(rotation().position().y() * M_PI / 180),
		sin(rotation().position().y() * M_PI / 180)
		).normalized() * length()
	};
}

Motion    ArmPart::rotation()    const { return marg_rotation_->motion(); }
Temperature ArmPart::temperature() const { return temperature_; }
Calibration        ArmPart::margCalibration()     const { return marg_calibration_; }
double      ArmPart::length()      const { return length_; }
