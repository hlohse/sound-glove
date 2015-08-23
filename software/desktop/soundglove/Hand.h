#pragma once

#include "ArmPart.h"
#include "HandTranslation.h"
#include "MARGRotation.h"
#include "Motion.h"
#include "Finger.h"
#include "Temperature.h"
#include "Calibration.h"
#include <memory>

class Hand {
	friend class Glove;
public:
	Hand();
	Hand(const std::shared_ptr<ArmPart>& forearm,
		 const std::shared_ptr<ArmPart>& upper_arm);

	SOUNDGLOVE_API Motion translation() const;
	SOUNDGLOVE_API Motion rotation() const;

	SOUNDGLOVE_API Finger thumb() const;
	SOUNDGLOVE_API Finger indexFinger() const;
	SOUNDGLOVE_API Finger middleFinger() const;
	SOUNDGLOVE_API Finger ringFinger() const;
	SOUNDGLOVE_API Finger littleFinger() const;

	SOUNDGLOVE_API Temperature temperature() const;

	SOUNDGLOVE_API Calibration margCalibration() const;

private:
	HandTranslation hand_translation_;
	std::shared_ptr<MARGRotation> marg_rotation_;
	Finger thumb_; // Technically incorrect :)
	Finger index_finger_;
	Finger middle_finger_;
	Finger ring_finger_;
	Finger little_finger_;
	Temperature temperature_;
	Calibration marg_calibration_;
};
