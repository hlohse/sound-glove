#include "Platform.h"
#include "BluetoothGuard.h"
#include "BluetoothSPP.h"
#include "Glove.h"
#include "Command.h"
#include "MeanCommand.h"
#include "MeanAnswer.h"
#include "Calibrate.h"
#include "SetCalibrationCommand.h"
#include "StopReadoutCommand.h"
#include "StartReadoutCommand.h"
#include "ReadoutCommand.h"
#include "MARGConfiguration.h"
#include "DummyCommand.h"
#include "IntegrateGCommand.h"
#include "IntegrateGAnswer.h"
#include "Timer.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <chrono>
#include <utility>
using namespace std;

static const string csv_path = "F:\\Studium\\MA\\glove\\thesis\\";
static const int m_gather_g_rot_s = 5; // Must be able to rotate by 90 degrees :)
static const int m_gather_xyz_num = 25000;
static const int m_gather_axis_num = 10000;

void gather(Context& context, const uint8_t sensor, array<Vector<int16_t>, 3>& calibration_calibration, const uint32_t fifos)
{
	MeanCommand mean_command;
	SharedPointer<Command> command = SharedPointer<Command>(new DummyCommand());

	cout << "Gathering data... ";

	mean_command.sensor = sensor;
	mean_command.fifos = fifos;
	mean_command.send(context);

	while (command->type() != Command::type_mean_answer) {
		command->execute(context);
		context.serial_port->waitUntilAvailable(Command::prefix_size);
		command = Command::received(context);
	}

	for (int i = 0; i < 3; ++i) {
		calibration_calibration[i] = (*((SharedPointer<MeanAnswer>*)&command))->sensor_calibration[i];
	}

	cout << "OK" << endl;
}

Vector<float> gatherG(Context& context, const int sensor, const Vector<int16_t>& offset)
{
	IntegrateGCommand command;
	SharedPointer<Command> answer = SharedPointer<Command>(new DummyCommand());
	command.sensor = (uint8_t)sensor;
	command.duration_s = (uint8_t)m_gather_g_rot_s;
	command.calibration_offset = offset;

	cout << "Gathering data... ";
	command.send(context);

	while (answer->type() != Command::type_integrate_g_answer) {
		context.serial_port->waitUntilAvailable(Command::prefix_size);
		answer = Command::received(context);
	}

	cout << "Done!" << endl;

	return (*((SharedPointer<IntegrateGAnswer>*)&answer))->rotation;
}

void rotateG(Context& context, array<Vector<float>, 6>& destination, const int sensor, const Vector<int16_t>& offset)
{
	cout << "Rotate device around positive x axis by 90deg. (Press enter to continue.)"; cin.ignore();
	destination[0] = gatherG(context, sensor, offset);
	cout << "Rotate device around negative x axis by 90deg. (Press enter to continue.)"; cin.ignore();
	destination[1] = gatherG(context, sensor, offset);
	cout << "Rotate device around positive y axis by 90deg. (Press enter to continue.)"; cin.ignore();
	destination[2] = gatherG(context, sensor, offset);
	cout << "Rotate device around negative y axis by 90deg. (Press enter to continue.)"; cin.ignore();
	destination[3] = gatherG(context, sensor, offset);
	cout << "Rotate device around positive z axis by 90deg. (Press enter to continue.)"; cin.ignore();
	destination[4] = gatherG(context, sensor, offset);
	cout << "Rotate device around negative z axis by 90deg. (Press enter to continue.)"; cin.ignore();
	destination[5] = gatherG(context, sensor, offset);
}

void gatherM(Context& context, vector<array<Vector<int16_t>, 3>>& destination, const int num)
{
	StopReadoutCommand stop_readout_command;
	StartReadoutCommand start_readout_command;

	cout << "Gathering data... ";
	start_readout_command.send(context);

	for (int i = 0; i < num; ++i) {
		SharedPointer<Command> command = SharedPointer<Command>(new DummyCommand());
		array<Vector<int16_t>, 3> calibration;

		while (command->type() != Command::type_readout) {
			command->execute(context);
			context.serial_port->waitUntilAvailable(Command::prefix_size);
			command = Command::received(context);
		}

		for (int i = 0; i < 3; ++i) {
			calibration[i] = (*((SharedPointer<ReadoutCommand>*)&command))->readout.margs[i].m;
		}

		destination.push_back(calibration);
	}

	stop_readout_command.send(context);
	this_thread::sleep_for(chrono::seconds(1));
	context.serial_port->clear();
	cout << "OK" << endl;
}

int main(int argc, char* argv[])
{
	Glove::initialize();

	BluetoothGuard bt_guard;
	const string name = argv[1];
	const int timeout_ms = atoi(argv[2]);

	try {
#ifdef LOG
		cout << "Opening serial port to " << name << " ";
#endif
		const shared_ptr<SerialPort> serial_port = make_shared<BluetoothSPP>(name, timeout_ms);
#ifdef LOG
		cout << "OK" << endl;
#endif

#ifdef LOG
		cout << "Setting up glove data... ";
#endif
		const shared_ptr<Glove> glove = make_shared<Glove>();
#ifdef LOG
		cout << "OK" << endl;
#endif

		Context context = { serial_port, glove, SharedPointer<Readout>(new Readout()) };

		SetCalibrationCommand set_calibration_command;
		Calibrate calibrate(csv_path);
		array<Calibration, 3> calibration;

		/*
		cout << "Lie device still on a flat surface. (Press enter to continue)"; cin.ignore();
		gather(context, MeanCommand::sensor_a, calibrate.mean_a[0], 50000);

		cout << "Lie device still on a flat surface, upside down. (Press enter to continue)"; cin.ignore();
		gather(context, MeanCommand::sensor_a, calibrate.mean_a[1], 50000);

		cout << "Stand device on Bluetooth module. (Press enter to continue)"; cin.ignore();
		gather(context, MeanCommand::sensor_a, calibrate.mean_a[2], 50000);

		cout << "Stand device on Bluetooth module, upside down. (Press enter to continue)"; cin.ignore();
		gather(context, MeanCommand::sensor_a, calibrate.mean_a[3], 50000);

		cout << "Stand device on JTAG port. (Press enter to continue)"; cin.ignore();
		gather(context, MeanCommand::sensor_a, calibrate.mean_a[4], 50000);

		cout << "Stand device on JTAG port, upside down. (Press enter to continue)"; cin.ignore();
		gather(context, MeanCommand::sensor_a, calibrate.mean_a[5], 50000);

		cout << "Lie device still on a flat surface. (Press enter to continue)"; cin.ignore();
		gather(context, MeanCommand::sensor_g, calibrate.mean_g, 250000);

		rotateG(context, calibrate.rot_g[0], IntegrateGCommand::sensor_hand, calibrate.mean_g[0]);
		rotateG(context, calibrate.rot_g[1], IntegrateGCommand::sensor_forearm, calibrate.mean_g[1]);
		rotateG(context, calibrate.rot_g[2], IntegrateGCommand::sensor_upper_arm, calibrate.mean_g[2]);
		
		cout << "Rotate device around magnetometer x axis. (Press enter to start)"; cin.ignore();
		gatherM(context, calibrate.raw_m_x, m_gather_axis_num);

		cout << "Rotate device around magnetometer y axis. (Press enter to start)"; cin.ignore();
		gatherM(context, calibrate.raw_m_y, m_gather_axis_num);

		cout << "Rotate device around magnetometer z axis. (Press enter to start)"; cin.ignore();
		gatherM(context, calibrate.raw_m_z, m_gather_axis_num);

		cout << "Rotate device around all axis. (Press enter to start)"; cin.ignore();
		gatherM(context, calibrate.raw_m_xyz, m_gather_xyz_num);

		cout << "Exporting with raw magnetometer values to \"" << csv_path << "\"" << endl;
		calibrate.exportRawM();
		*/

		calibrate.importBasic();
		//calibrate.determineCalibration();
		//calibrate.exportBasic();
		cout << "Prepare magnetometer calibration file at \"" << csv_path << "\". (Press enter to continue.)" << endl; cin.ignore();
		calibrate.importCalibrationM();

		calibration = calibrate.calibration;
		cout << "Calibration" << endl;
		cout << "Hand      GO " << calibration[0].g_offset.x() << " " << calibration[0].g_offset.y() << " " << calibration[0].g_offset.z() << endl;
		cout << "          GS " << calibration[0].g_scale.x() << " " << calibration[0].g_scale.y() << " " << calibration[0].g_scale.z() << endl;
		cout << "          AO " << calibration[0].a_offset.x() << " " << calibration[0].a_offset.y() << " " << calibration[0].a_offset.z() << endl;
		cout << "          AS " << calibration[0].a_scale.x() << " " << calibration[0].a_scale.y() << " " << calibration[0].a_scale.z() << endl;
		cout << "          MO " << calibration[0].m_offset.x() << " " << calibration[0].m_offset.y() << " " << calibration[0].m_offset.z() << endl;
		cout << "          MR " << calibration[0].m_rotation.get(0, 0) << " " << calibration[0].m_rotation.get(0, 1) << " " << calibration[0].m_rotation.get(0, 2) << endl;
		cout << "             " << calibration[0].m_rotation.get(1, 0) << " " << calibration[0].m_rotation.get(1, 1) << " " << calibration[0].m_rotation.get(1, 2) << endl;
		cout << "             " << calibration[0].m_rotation.get(2, 0) << " " << calibration[0].m_rotation.get(2, 1) << " " << calibration[0].m_rotation.get(2, 2) << endl;
		cout << "Forearm GO " << calibration[1].g_offset.x() << " " << calibration[1].g_offset.y() << " " << calibration[1].g_offset.z() << endl;
		cout << "          GS " << calibration[1].g_scale.x() << " " << calibration[1].g_scale.y() << " " << calibration[1].g_scale.z() << endl;
		cout << "          AO " << calibration[1].a_offset.x() << " " << calibration[1].a_offset.y() << " " << calibration[1].a_offset.z() << endl;
		cout << "          AS " << calibration[1].a_scale.x() << " " << calibration[1].a_scale.y() << " " << calibration[1].a_scale.z() << endl;
		cout << "          MO " << calibration[1].m_offset.x() << " " << calibration[1].m_offset.y() << " " << calibration[1].m_offset.z() << endl;
		cout << "          MR " << calibration[1].m_rotation.get(0, 0) << " " << calibration[1].m_rotation.get(0, 1) << " " << calibration[1].m_rotation.get(0, 2) << endl;
		cout << "             " << calibration[1].m_rotation.get(1, 0) << " " << calibration[1].m_rotation.get(1, 1) << " " << calibration[1].m_rotation.get(1, 2) << endl;
		cout << "             " << calibration[1].m_rotation.get(2, 0) << " " << calibration[1].m_rotation.get(2, 1) << " " << calibration[1].m_rotation.get(2, 2) << endl;
		cout << "Upper Arm GO " << calibration[2].g_offset.x() << " " << calibration[2].g_offset.y() << " " << calibration[2].g_offset.z() << endl;
		cout << "          GS " << calibration[2].g_scale.x() << " " << calibration[2].g_scale.y() << " " << calibration[2].g_scale.z() << endl;
		cout << "          AO " << calibration[2].a_offset.x() << " " << calibration[2].a_offset.y() << " " << calibration[2].a_offset.z() << endl;
		cout << "          AS " << calibration[2].a_scale.x() << " " << calibration[2].a_scale.y() << " " << calibration[2].a_scale.z() << endl;
		cout << "          MO " << calibration[2].m_offset.x() << " " << calibration[2].m_offset.y() << " " << calibration[2].m_offset.z() << endl;
		cout << "          MR " << calibration[2].m_rotation.get(0, 0) << " " << calibration[2].m_rotation.get(0, 1) << " " << calibration[2].m_rotation.get(0, 2) << endl;
		cout << "             " << calibration[2].m_rotation.get(1, 0) << " " << calibration[2].m_rotation.get(1, 1) << " " << calibration[2].m_rotation.get(1, 2) << endl;
		cout << "             " << calibration[2].m_rotation.get(2, 0) << " " << calibration[2].m_rotation.get(2, 1) << " " << calibration[2].m_rotation.get(2, 2) << endl;
		
		set_calibration_command.calibration[0] = calibration[0];
		set_calibration_command.calibration[1] = calibration[1];
		set_calibration_command.calibration[2] = calibration[2];
		set_calibration_command.send(context);

		cout << "Calibration data sent. Done!";
		cin.ignore();
	}
	catch (runtime_error e) {
#ifdef LOG
		cout << "ERROR!" << endl;
		cout << e.what() << endl;
		cin.ignore();
#endif
	}
	return 0;
}

