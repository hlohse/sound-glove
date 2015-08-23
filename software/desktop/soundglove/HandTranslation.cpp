#include "HandTranslation.h"
#include "Vector.h"
using namespace std;

HandTranslation::HandTranslation(const shared_ptr<ArmPart>& forearm,
	                             const shared_ptr<ArmPart>& upper_arm)
:	forearm_(forearm),
	upper_arm_(upper_arm)
{
}

void HandTranslation::update(const MARG& marg, const double period_s)
{
	const Vector<double> position = upper_arm_->toVector() + forearm_->toVector();
	const Vector<double> acceleration = marg.a();
	const Vector<double> speed = (position - motion_.position()) / period_s;
	motion_ = { position, speed, acceleration };
}

Motion HandTranslation::motion() const
{
	return motion_;
}
