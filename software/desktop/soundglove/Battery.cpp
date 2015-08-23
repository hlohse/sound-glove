#include "Battery.h"

const double Battery::threshold_voltage_full = 4;
const double Battery::threshold_voltage_empty = 3.5;

Battery::Battery()
:   charge_state_(ChargeState::Unknown),
	usage_state_(UsageState::Unknown),
	charge_(0.0)
{
}

Battery::ChargeState Battery::chargeState()  const
{
	return charge_state_;
}

Battery::UsageState Battery::usageState()  const
{
	return usage_state_;
}

double Battery::charge() const
{
	return charge_;
}

double Battery::percentage() const
{
	if (charge() > threshold_voltage_full) {
		return 100;
	}
	else if (charge() < threshold_voltage_empty) {
		return 0;
	}
	else {
		return 100 * (charge() - threshold_voltage_empty) /
			(threshold_voltage_full - threshold_voltage_empty);
	}
}

void Battery::setCharge(const int adc_voltage, const double is_used)
{
	const double adc = ((double)adc_voltage) / 4096;
	charge_ = adc * 2.7 / (27.0 / (27.0 + 56.0));

	if (is_used) {
		usage_state_ = UsageState::InUse;
		charge_state_ = ChargeState::Discharging;
	}
	else {
		usage_state_ = UsageState::PluggedIn;
		charge_state_ = percentage() >= 100 ? ChargeState::Full : ChargeState::Charging;
	}
}
