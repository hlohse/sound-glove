#include "Platform.h"
#include "BluetoothGuard.h"
#include "BluetoothSPP.h"
#include "Glove.h"
#include "Command.h"
#include "StartReadoutCommand.h"
#include "StopReadoutCommand.h"
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

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

#ifdef LOG
		cout << "Command loop running!" << endl;
#endif

		StartReadoutCommand start;
		StopReadoutCommand stop;

		for (;;) {
			start.send(context);

			context.serial_port->waitUntilAvailable(Command::prefix_size);
			Command::received(context)->execute(context);

			cout << "Hand" << endl;
			cout << "    temp     [degC]   " << glove->hand().temperature().degreesCelsius() << endl;
			cout << "    x        [mm]     " << glove->hand().translation().position().x() << endl;
			cout << "    y        [mm]     " << glove->hand().translation().position().y() << endl;
			cout << "    z        [mm]     " << glove->hand().translation().position().z() << endl;
			cout << "    roll     [deg]    " << glove->hand().rotation().position().x() << endl;
			cout << "    pitch    [deg]    " << glove->hand().rotation().position().y() << endl;
			cout << "    yaw      [deg]    " << glove->hand().rotation().position().z() << endl;
			cout << "Forearm" << endl;
			cout << "    length   [mm]     " << glove->forearm().length() << endl;
			cout << "    temp     [degC]   " << glove->forearm().temperature().degreesCelsius() << endl;
			cout << "    roll     [deg]    " << glove->forearm().rotation().position().x() << endl;
			cout << "    pitch    [deg]    " << glove->forearm().rotation().position().y() << endl;
			cout << "    yaw      [deg]    " << glove->forearm().rotation().position().z() << endl;
			cout << "Upper Arm" << endl;
			cout << "    length   [mm]     " << glove->upperArm().length() << endl;
			cout << "    temp     [degC]   " << glove->upperArm().temperature().degreesCelsius() << endl;
			cout << "    roll     [deg]    " << glove->upperArm().rotation().position().x() << endl;
			cout << "    pitch    [deg]    " << glove->upperArm().rotation().position().y() << endl;
			cout << "    yaw      [deg]    " << glove->upperArm().rotation().position().z() << endl;
			cout << endl;

			stop.send(context);
		}
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

