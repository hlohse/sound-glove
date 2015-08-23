#pragma once

#include "Calibration.h"
#include "Vector.h"
#include "Export.h"
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdint>

struct Calibrate {
	std::array<Calibration, 3> calibration;

	// Temperatures during calibration (stabilize first!)
	std::array<int16_t, 3> temperatures;

	// Mean gyroscope values if the device was not moved
	std::array<Vector<int16_t>, 3> mean_g;

	// Gyroscope values if the device was rotated by 90°
	// 0/1: x -> -x, -x -> x
	// 2/3: y -> -y, -y -> y
	// 4/5: z -> -z, -z -> z
	std::array<std::array<Vector<float>, 6>, 3> rot_g;

	/* Mean accelerometer values if the device was not moved
	 * 0-1: Lying on a flat surface + upside down
	 * 2-3: Orthogonal to lying, Bluetooth module at bottom + upside down
	 * 4-5: Orthogonal to lying, JTAG port at bottom + upside down
	 */
	std::array<std::array<Vector<int16_t>, 3>, 6> mean_a;

	// Uncalibrated magnetometer values over time if the device was rotated around x axis
	std::vector<std::array<Vector<int16_t>, 3>> raw_m_x;

	// Uncalibrated magnetometer values over time if the device was rotated around y axis
	std::vector<std::array<Vector<int16_t>, 3>> raw_m_y;

	// Uncalibrated magnetometer values over time if the device was rotated around z axis
	std::vector<std::array<Vector<int16_t>, 3>> raw_m_z;

	// Uncalibrated magnetometer values over time if the device was rotated in 3 dimensions
	std::vector<std::array<Vector<int16_t>, 3>> raw_m_xyz;

	SOUNDGLOVE_API Calibrate(const std::string& csv_path);

	SOUNDGLOVE_API void exportBasic() const;
	SOUNDGLOVE_API void exportRawM() const;
	SOUNDGLOVE_API void determineCalibration();
	SOUNDGLOVE_API void importBasic();
	SOUNDGLOVE_API void importCalibrationM();

private:
	// Accelerometer 1G readout at 16G range
	static const int a_gravition_16g = 1366;
	static const int g_deg_per_axis = 90;
	static const char csv_seperator = ' ';

	std::string csv_path_;

	void determineHandCalibration();
	void determineForearmCalibration();
	void determineUpperArmCalibration();

	void determineCalibration(const int index);
	void determineCalibrationG(const int index);
	void determineCalibrationA(const int index);

	void exportBasic(const int index, const std::string& label) const;
	void exportRawM(const int index, const std::string& label) const;
};
