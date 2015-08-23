#pragma once

#include "Export.h"
#include "SerialPort.h"
#include "Hand.h"
#include "ArmPart.h"
#include "Readout.h"
#include "Temperature.h"
#include "Vector.h"
#include "Battery.h"
#include "Timer.h"
#include "System.h"
#include <memory>
#include <string>

class Glove {
	friend class ReadoutCommand;
	friend class TemperatureCommand;
	friend class HeadingAnswer;
	friend class SendCalibrationCommand;
	friend class SendBatteryChargeCommand;

public:
	// Call once first!
	SOUNDGLOVE_API static void initialize();

	SOUNDGLOVE_API Glove(const double ahrs_beta = 0.041,
		                 const double ahrs_zeta = 0.5,
						 const std::string& packet_rate_filename = "packets.csv");

	SOUNDGLOVE_API Hand hand() const;
	SOUNDGLOVE_API ArmPart forearm() const;
	SOUNDGLOVE_API ArmPart upperArm() const;
	SOUNDGLOVE_API Battery battery() const;
	SOUNDGLOVE_API bool isCapacitivePinTouched() const;

	SOUNDGLOVE_API int readoutRate() const; // [readouts/s]

private:
	static const int num_readouts_to_drop_first = 100;
	static const int calibration_delay_s = 10;

	std::shared_ptr<ArmPart> forearm_;
	std::shared_ptr<ArmPart> upper_arm_;
	Hand hand_;
	Battery battery_;
	bool is_capacitive_pin_touched_;
	Timer timer_readout_;
	Timer timer_calibration_;
	int dropped_readouts_;
	bool is_calibrated_;
	int readouts_;
	int readout_rate_;
	double ahrs_beta_;
	double ahrs_zeta_;
	std::string packet_rate_filename_;
#ifdef LOG_ACCURACY
	Timer timer_accuracy_;
#endif

	void setCalibration(const Calibration calibration[3]);

	void initializeHeading(const Vector<int16_t> a[3],
					       const Vector<int16_t> m[3]);

	void update(const Readout& readout);
	void update(const int16_t temperatures[3]);
	void update(const int battery_charge, const bool is_used);
};
