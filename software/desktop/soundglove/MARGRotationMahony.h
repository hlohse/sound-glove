#pragma once 

#include "MARGRotation.h"
#include "Quaternion.h"
#include "Vector.h"

class MARGRotationMahony : public MARGRotation {
public:
	MARGRotationMahony();

private:
	static const double gain_proportional;
	static const double gain_integral;

	Vector<double> feedback_;

	virtual void updateQuaternion(const MARG& marg, const double period_s);
	void applyFeedback(const MARG& marg, Vector<double>& g, const double period_s);

	/* There algorithms are derived from Mahonys's MARG algorithm.
	* For more details, see:
	* http://www.x-io.co.uk/node/8#open_source_ahrs_and_marg_algorithms
	*/
	void applyFeedbackMarg(const MARG& marg, Vector<double>& g, const double period_s);
	void applyFeedbackImu(const MARG& marg, Vector<double>& g, const double period_s);
	void applyFeedback(const MARG& marg, const Vector<double>& error, Vector<double>& g, const double period_s);
};
