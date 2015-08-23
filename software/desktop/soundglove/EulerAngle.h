#pragma once

struct Quaternion;

// [deg] roll-pitch-yaw order (Tait-Bryan), -180..180deg
struct EulerAngle {
	double roll;
	double pitch;
	double yaw;

	EulerAngle();
	EulerAngle(const double roll,
		       const double pitch,
		       const double yaw);
	EulerAngle(const Quaternion& quaternion);

	EulerAngle operator-(const EulerAngle& other) const;

private:
	static const double epsilon;

	void adjustRange(double& angle);
};
