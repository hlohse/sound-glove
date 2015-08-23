#pragma once

#include "ArmPart.h"
#include "MARG.h"
#include "Motion.h"
#include <memory>

class HandTranslation {
	friend class Glove;
public:
	HandTranslation(const std::shared_ptr<ArmPart>& forearm,
		            const std::shared_ptr<ArmPart>& upper_arm);

	Motion motion() const;

private:
	std::shared_ptr<ArmPart> forearm_;
	std::shared_ptr<ArmPart> upper_arm_;
	Motion motion_;

	void update(const MARG& marg, const double period_s);
};
