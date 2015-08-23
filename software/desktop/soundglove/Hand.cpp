#include "Hand.h"
#include "MARGRotationFactory.h"

Hand::Hand()
:	Hand({ nullptr }, { nullptr })
{
}

Hand::Hand(const std::shared_ptr<ArmPart>& forearm,
	       const std::shared_ptr<ArmPart>& upper_arm)
:	hand_translation_(forearm, upper_arm),
	marg_rotation_(MARGRotationFactory::getImuRotation()),
	marg_calibration_()
{
}

Motion    Hand::translation()  const { return hand_translation_.motion(); }
Motion    Hand::rotation()     const { return marg_rotation_->motion(); }
Finger      Hand::thumb()        const { return thumb_; }
Finger      Hand::indexFinger()  const { return index_finger_; }
Finger      Hand::middleFinger() const { return middle_finger_; }
Finger      Hand::ringFinger()   const { return ring_finger_; }
Finger      Hand::littleFinger() const { return little_finger_; }
Temperature Hand::temperature()  const { return temperature_; }
Calibration        Hand::margCalibration()      const { return marg_calibration_; }
