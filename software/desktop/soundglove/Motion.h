#pragma once

#include "Vector.h"

class Motion {
public:
	SOUNDGLOVE_API Motion();
	SOUNDGLOVE_API Motion(const Vector<double>& position,
							const Vector<double>& speed,
							const Vector<double>& acceleration);

	SOUNDGLOVE_API Vector<double> position() const;     // [cm]     or [deg]
	SOUNDGLOVE_API Vector<double> speed() const;        // [cm/s]   or [deg/s]
	SOUNDGLOVE_API Vector<double> acceleration() const; // [cm/s^2] or [deg/s^2]

private:
	Vector<double> position_;
	Vector<double> speed_;
	Vector<double> acceleration_;
};
