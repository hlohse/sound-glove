#include "Platform.h"
#include "BluetoothGuard.h"
#include "BluetoothSPP.h"
#include "Glove.h"
#include "Command.h"
#include "Visualization.h"
#include "StartReadoutCommand.h"
#include "Timer.h"
#include "MidiPort.h"
#include "MidiController.h"
#include "MIDINote.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

static const double fps = 30;
static const double fps_period = 1 / fps;

int main(int argc, char* argv[])
{
	Glove::initialize();

	BluetoothGuard bt_guard;
	Timer timer;
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
		const shared_ptr<Glove> glove = make_shared<Glove>(0.5, 0.5);
#ifdef LOG
		cout << "OK" << endl;
#endif

		Context context = { serial_port, glove, SharedPointer<Readout>(new Readout()) };
		Visualization visualization(context, 1000, 700);
		MidiPort midi_port("Sound Glove");

#ifdef LOG
		cout << "Command loop running!" << endl;
#endif
		MIDINote theremin(
			[&]() { return (int)(57 + (glove->hand().translation().position().z() + 55) / 100); },
			[&]() { return (int)(127 - glove->hand().indexFinger().flex() * 1.27); }
		);

		StartReadoutCommand start;
		start.send(context);
		timer.start();

		vector<double> durations;

		for (;;) {
			serial_port->waitUntilAvailable(Command::prefix_size);
			if (Command::hasReceived(context, Command::type_readout)) {
				Timer timer;
				timer.start();
				Command::received(context)->execute(context);
				timer.stop();
				durations.push_back(timer.duration());

				midi_port.Play(theremin);
			}
			else {
				Command::received(context)->execute(context);
			}

			if (durations.size() == 1000) {
				volatile double total_duration = 0;
				for each (double duration in durations) {
					total_duration += duration;
				}
				total_duration /= 1000;
			}

			timer.stop();
			if (timer.duration() >= fps_period) {
				visualization.update();
				timer.start();
			}
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

