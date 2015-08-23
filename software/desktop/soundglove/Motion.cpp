#include "Motion.h"

Motion::Motion()
{
}

Motion::Motion(const Vector<double>& position,
				   const Vector<double>& speed,
				   const Vector<double>& acceleration)
:	position_(position),
	speed_(speed),
	acceleration_(acceleration)
{
}

Vector<double> Motion::position()     const { return position_; }
Vector<double> Motion::speed()        const { return speed_; }
Vector<double> Motion::acceleration() const { return acceleration_; }
