#pragma once

#include "Export.h"

class Battery {
	friend class Glove;

public:
	enum class ChargeState {
		Unknown,
		Discharging,
		Charging,
		Full
	};

	enum class UsageState {
		Unknown,
		InUse,
		PluggedIn
	};

	Battery();

	SOUNDGLOVE_API ChargeState chargeState() const;
	SOUNDGLOVE_API UsageState usageState() const;
	SOUNDGLOVE_API double charge() const;     // [V]
	SOUNDGLOVE_API double percentage() const; // [%]

private:
	// [V], used to estimate charge perventage. Refer to linear regression.
	static const double threshold_voltage_full;
	static const double threshold_voltage_empty;

	ChargeState charge_state_;
	UsageState usage_state_;
	double charge_;

	void setCharge(const int adc_voltage, const double is_used);
};
