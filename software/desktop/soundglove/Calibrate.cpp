#include "Calibrate.h"
#include "MARGConfiguration.h"
#include <climits>
#include <cmath>
using namespace std;

Calibrate::Calibrate(const string& csv_path)
:	csv_path_(csv_path),
	calibration(),
	mean_g(),
	mean_a(),
	raw_m_xyz()
{
}

void Calibrate::importBasic()
{
	ifstream hand_file(csv_path_ + "hand_basic.csv");
	ifstream forearm_file(csv_path_ + "forearm_basic.csv");
	ifstream upper_arm_file(csv_path_ + "upper_arm_basic.csv");
	if (hand_file && forearm_file && upper_arm_file) {
		hand_file >> calibration[0].temperature;
		forearm_file >> calibration[1].temperature;
		upper_arm_file >> calibration[2].temperature;

		calibration[1].has_inverted_placement = true;
		calibration[2].has_inverted_placement = true;

		for (int i = 0; i < 3; ++i) {
			hand_file >> mean_g[0].values[i];
			forearm_file >> mean_g[1].values[i];
			upper_arm_file >> mean_g[2].values[i];
			calibration[0].g_offset.values[i] = mean_g[0].values[i];
			calibration[1].g_offset.values[i] = mean_g[1].values[i];
			calibration[2].g_offset.values[i] = mean_g[2].values[i];
		}

		for (int i = 0; i < 3; ++i) {
			hand_file >> calibration[0].g_scale.values[i];
			forearm_file >> calibration[1].g_scale.values[i];
			upper_arm_file >> calibration[2].g_scale.values[i];
		}

		for (int i = 0; i < 3; ++i) {
			hand_file >> calibration[0].a_offset.values[i];
			forearm_file >> calibration[1].a_offset.values[i];
			upper_arm_file >> calibration[2].a_offset.values[i];
		}

		for (int i = 0; i < 3; ++i) {
			hand_file >> calibration[0].a_scale.values[i];
			forearm_file >> calibration[1].a_scale.values[i];
			upper_arm_file >> calibration[2].a_scale.values[i];
		}

		hand_file.close();
		forearm_file.close();
		upper_arm_file.close();
	}
	else {
		cout << "ERROR: Cannot open raw calibrate csv files!" << endl;
	}
}

void Calibrate::importCalibrationM()
{
	ifstream file(csv_path_ + "magnetometer.csv");
	if (file) {
		int16_t offset = 0;
		float rotation = 0;
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				file >> offset;
				calibration[i].m_offset.values[j] = offset;
			}
		}
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				for (int k = 0; k < 3; ++k) {
					file >> rotation;
					calibration[i].m_rotation.values[j * 3 + k] = rotation;
				}
			}
		}
		file.close();
	}
	else {
		cout << "ERROR: Cannot open raw calibrate csv files!" << endl;
	}
}

void Calibrate::determineCalibration()
{
	determineHandCalibration();
	determineForearmCalibration();
	determineUpperArmCalibration();
}

void Calibrate::determineHandCalibration()
{
	calibration[0].has_inverted_placement = false;
	determineCalibration(0);
}

void Calibrate::determineForearmCalibration()
{
	calibration[1].has_inverted_placement = true;
	determineCalibration(1);
}

void Calibrate::determineUpperArmCalibration()
{
	calibration[2].has_inverted_placement = true;
	determineCalibration(2);
}

void Calibrate::determineCalibration(const int index)
{
	determineCalibrationG(index);
	determineCalibrationA(index);
}

void Calibrate::determineCalibrationG(const int index)
{
	/* Gyroscope values are expected to be 0 if the device lies still.
	 * Therefore, measured offset is calibration.
	 * Determine scaling factors by integrating the values to form degree.
	 * Compare this to expected 90°.
	 */
	calibration[index].g_offset = mean_g[index];
	calibration[index].g_scale.setX(g_deg_per_axis / ((abs(rot_g[index][0].x()) + abs(rot_g[index][1].x())) / 2));
	calibration[index].g_scale.setY(g_deg_per_axis / ((abs(rot_g[index][2].y()) + abs(rot_g[index][3].y())) / 2));
	calibration[index].g_scale.setZ(g_deg_per_axis / ((abs(rot_g[index][4].z()) + abs(rot_g[index][5].z())) / 2));
}

void Calibrate::determineCalibrationA(const int index)
{
	/* Top view:       Side view:
	 *       -y       
	 *     ------        
	 * +x |     o| -x      -z
	 *    |      |      |-----o|  top
	 *     ------      ---------- bottom
	 *       +y            +z
	 *
	 * Axis expected to be 0 (= not affected by gravity):
	 * 0..1 (lying + upside down):      X Y
	 * 2..3 (ortho BT + upside down):   X Z
	 * 4..5 (ortho JTAG + upside down): Y Z
	 */
	calibration[index].a_offset = Vector<int16_t>(
		mean_a[0][index].x() + mean_a[1][index].x() + mean_a[2][index].x() + mean_a[3][index].x(),
		mean_a[0][index].y() + mean_a[1][index].y() + mean_a[4][index].y() + mean_a[5][index].y(),
		mean_a[2][index].z() + mean_a[3][index].z() + mean_a[4][index].z() + mean_a[5][index].z()
	) / 4;
	calibration[index].a_scale = Vector<float>(
		((float)a_gravition_16g) / (((abs((float)mean_a[4][index].x()) + abs((float)mean_a[5][index].x())) / 2) - calibration[index].a_offset.x()),
		((float)a_gravition_16g) / (((abs((float)mean_a[2][index].y()) + abs((float)mean_a[3][index].y())) / 2) - calibration[index].a_offset.y()),
		((float)a_gravition_16g) / (((abs((float)mean_a[0][index].z()) + abs((float)mean_a[1][index].z())) / 2) - calibration[index].a_offset.z())
	);
}

void Calibrate::exportBasic() const
{
	exportBasic(0, "hand");
	exportBasic(1, "forearm");
	exportBasic(2, "upper_arm");
}

void Calibrate::exportRawM() const
{
	exportRawM(0, "hand");
	exportRawM(1, "forearm");
	exportRawM(2, "upper_arm");
}

void Calibrate::exportBasic(const int index, const std::string& label) const
{
	ofstream t_g_a_mh(csv_path_ + label + "_basic.csv");
	if (t_g_a_mh) {

		//t_g_a_mh << "Temperatures" << endl;
		t_g_a_mh << calibration[index].temperature << endl;

		//t_g_a_mh << "Gyroscope offset " << endl;
		t_g_a_mh << calibration[index].g_offset.x() << csv_seperator << calibration[index].g_offset.y() << csv_seperator << calibration[index].g_offset.z() << endl;

		//t_g_a_mh << "Gyroscope scale " << endl;
		t_g_a_mh << calibration[index].g_scale.x() << csv_seperator << calibration[index].g_scale.y() << csv_seperator << calibration[index].g_scale.z() << endl;

		//t_g_a_mh << "Accelerometer offset" << endl;
		t_g_a_mh << calibration[index].a_offset.x() << csv_seperator << calibration[index].a_offset.y() << csv_seperator << calibration[index].a_offset.z() << endl;

		//t_g_a_mh << "Accelerometer scale" << endl;
		t_g_a_mh << calibration[index].a_scale.x() << csv_seperator << calibration[index].a_scale.y() << csv_seperator << calibration[index].a_scale.z() << endl;

		t_g_a_mh.close();
	}
	else {
		cout << "ERROR: Cannot open raw calibrate csv files!" << endl;
	}
}

void Calibrate::exportRawM(const int index, const std::string& label) const
{
	ofstream m_raw_x(csv_path_ + label + "_m_raw_x.csv");
	ofstream m_raw_y(csv_path_ + label + "_m_raw_y.csv");
	ofstream m_raw_z(csv_path_ + label + "_m_raw_z.csv");
	ofstream m_raw_all(csv_path_ + label + "_m_raw_all.csv");

	if (m_raw_x && m_raw_y && m_raw_z && m_raw_all) {
		for (auto it : raw_m_x) {
			m_raw_x << it[index].x() << csv_seperator << it[index].y() << csv_seperator << it[index].z() << endl;
		}
		for (auto it : raw_m_y) {
			m_raw_y << it[index].x() << csv_seperator << it[index].y() << csv_seperator << it[index].z() << endl;
		}
		for (auto it : raw_m_z) {
			m_raw_z << it[index].x() << csv_seperator << it[index].y() << csv_seperator << it[index].z() << endl;
		}
		for (auto it : raw_m_xyz) {
			m_raw_all << it[index].x() << csv_seperator << it[index].y() << csv_seperator << it[index].z() << endl;
		}

		m_raw_x.close();
		m_raw_y.close();
		m_raw_z.close();
		m_raw_all.close();
	}
	else {
		cout << "ERROR: Cannot open raw calibrate csv files!" << endl;
	}
}
