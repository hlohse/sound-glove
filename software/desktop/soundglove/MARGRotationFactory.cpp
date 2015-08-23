#include "MARGRotationFactory.h"
#include "MARGRotationMadgwick.h"
#include "MARGRotationMahony.h"
using namespace std;

shared_ptr<MARGRotation> MARGRotationFactory::getImuRotation()
{
	if (use_madgwick) {
		return make_shared<MARGRotationMadgwick>();
	}
	else {
		return make_shared<MARGRotationMahony>();
	}
}
