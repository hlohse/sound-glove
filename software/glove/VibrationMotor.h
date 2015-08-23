#ifndef VIBRATIONMOTOR_H_
#define VIBRATIONMOTOR_H_

#include "Pins.h"

class VibrationMotor {
public:
	static VibrationMotor& instance();
	static void instancePwm(void* parameters); // For Scheduler

	bool isOn() const;

	void on();
	void off();

	void toggle();

private:
	const Pin pin_;
	bool is_on_;

	VibrationMotor(const Pin& pin);

	// Disallow copies; (do not implement!)
	VibrationMotor(const VibrationMotor&);
	void operator=(const VibrationMotor&);
};

#endif /* VIBRATIONMOTOR_H_ */
