#pragma once

#include "MARGRotation.h"
#include <memory>

class MARGRotationFactory {
public:
	static std::shared_ptr<MARGRotation> getImuRotation();

private:
	/* use_madgwick = true  => MARGRotationMadgwick
	 * use_madgwick = false => MARGRotationMahony
	 */
	static const bool use_madgwick = true;
};
