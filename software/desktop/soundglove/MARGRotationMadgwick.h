#pragma once 

#include "MARGRotation.h"
#include "Quaternion.h"

class MARGRotationMadgwick : public MARGRotation {
public:
	MARGRotationMadgwick();

	double beta() const;
	void setBeta(const double beta);

	double zeta() const;
	void setZeta(const double zeta);

private:
	double beta_;
	double zeta_;
	Vector<double> g_drift_;

	virtual void updateQuaternion(const MARG& marg, const double period_s);
	Quaternion getFeedback(const MARG& marg, const double period_s);

	/* There algorithms are derived from Madgwick's MARG algorithm.
	* For more details, see:
	* http://www.x-io.co.uk/node/8#open_source_ahrs_and_marg_algorithms
	*/
	Quaternion getFeedbackMarg(const MARG& marg, const double period_s);
	Quaternion getFeedbackImu(const MARG& marg, const double period_s);
};
