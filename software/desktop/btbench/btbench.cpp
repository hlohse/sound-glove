#include "Platform.h"
#include "BluetoothGuard.h"
#include "BluetoothSPP.h"
#include "Command.h"
#include "Glove.h"
#include "HandshakeCommand.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <iomanip>
#include <cfloat>
#include <cmath>
#include <chrono>
#include <windows.h>
#include <fstream>
using namespace std;
using namespace chrono;

using TimeTuple = pair<LARGE_INTEGER, LARGE_INTEGER>;

static const string filename = "latency.csv";
static const int warmups = BENCHMARK_BLUETOOTH_LATENCY_WARMUPS;
static const int iterations = BENCHMARK_BLUETOOTH_LATENCY_ITERATIONS;
static const double runtime_s = 2;
static const double runtime_warmups_s = runtime_s * warmups / iterations;
static const int bt_read_size = BluetoothSPP::read_buffer_size / 2;
static double performance_counter_frequency = 1;
static Context context;

struct Result {
	double min;
	double max;
	double mean;
	double median;
	double stddev;
	double var;
};

void determineLatency(vector<TimeTuple>& times)
{
	static HandshakeCommand command;
	LARGE_INTEGER start, stop;
	times.clear();
	times.reserve(iterations);

	// Warmups
	for (int i = 0; i < warmups; ++i) {
		command.send(context);
		context.serial_port->waitUntilAvailable(Command::prefix_size);

		SharedPointer<Command> command = Command::received(context);
		if (command->type() == Command::type_handshake) {
			command->execute(context);
		}
	}

	// Benchmark
	for (int i = 0; i < iterations; ++i) {
		QueryPerformanceCounter(&start);
		command.send(context);
		context.serial_port->waitUntilAvailable(Command::prefix_size);
		QueryPerformanceCounter(&stop);

		SharedPointer<Command> command = Command::received(context);
		if (command->type() == Command::type_handshake) {
			command->execute(context);
			times.push_back(TimeTuple(start, stop));
		}
	}
}

vector<double> determineMs(const vector<TimeTuple>& times)
{
	vector<double> times_ms;
	times_ms.reserve(times.size());

	for (auto it = times.cbegin(); it != times.cend(); ++it) {
		const double start = (double)it->first.QuadPart;
		const double stop = (double)it->second.QuadPart;
		const double duration_ms = (stop - start) / performance_counter_frequency * 1e3;
		times_ms.push_back(duration_ms);
	}

	return times_ms;
}

Result getResult(vector<double>& values)
{
	vector<double> times_ms_sorted(values);
	Result result;

	sort(times_ms_sorted.begin(), times_ms_sorted.end());

	result.min = DBL_MAX;
	result.max = DBL_MIN;
	result.mean = 0;
	result.median = values[times_ms_sorted.size() / 2];
	result.var = 0;

	for (auto it = values.cbegin(); it != values.cend(); ++it) {
		const double time = *it;

		if (time < result.min) {
			result.min = time;
		}
		if (time > result.max) {
			result.max = time;
		}

		result.mean += time;
	}

	result.mean /= values.size();

	for (auto it = values.cbegin(); it != values.cend(); ++it) {
		const double time = *it;
		result.var += (time - result.mean) * (time - result.mean);
	}

	result.var /= values.size();
	result.stddev = sqrt(result.var);

	return result;
}

void show(const vector<double>& times_ms, const string& unit)
{
	ofstream file(filename, fstream::app);
	cout << setprecision(2) << fixed;
	for (auto it = times_ms.cbegin(); it != times_ms.cend(); ++it) {
		cout << it - times_ms.begin() << " " << *it << " " << unit << endl;
		if (file) {
			file << it - times_ms.begin() << " " << *it << " " << unit << endl;
		}
	}
	file.close();
}

void show(const Result& result, const string& unit)
{
	ofstream file(filename, fstream::app);
	cout << setprecision(2) << fixed;
	cout << "min\tmax\tmean\tmedian\tstddev\tvar\t[" << unit << "]" << endl;
	cout << result.min << "\t"
		<< result.max << "\t"
		<< result.mean << "\t"
		<< result.median << "\t"
		<< result.stddev << "\t"
		<< result.var << endl;
	if (file) {
		file << "min\tmax\tmean\tmedian\tstddev\tvar\t[" << unit << "]" << endl;
		file << result.min << "\t"
			<< result.max << "\t"
			<< result.mean << "\t"
			<< result.median << "\t"
			<< result.stddev << "\t"
			<< result.var << endl;
		file.close();
	}
}

int main(int argc, char* argv[])
{
	Glove::initialize();

	BluetoothGuard bt_guard;
	const string name = argv[1];
	const int timeout_ms = atoi(argv[2]);
	LARGE_INTEGER frequency;
	vector<TimeTuple> times;
	vector<double> times_ms;
	Result result_latency;

	try {
#ifdef LOG
		cout << "Opening serial port to " << name << "... ";
#endif
		shared_ptr<BluetoothSPP> serial_port = make_shared<BluetoothSPP>(name, timeout_ms);
#ifdef LOG
		cout << "OK" << endl;
#endif

#ifdef LOG
		cout << "Setting up glove data... ";
#endif
		shared_ptr<Glove> glove = make_shared<Glove>();
#ifdef LOG
		cout << "OK" << endl;
#endif

		context = { serial_port, glove, SharedPointer<Readout>(new Readout()) };

#ifdef LOG
		cout << "Starting benchmark!" << endl;
#endif

		QueryPerformanceFrequency(&frequency);
		performance_counter_frequency = (double) frequency.QuadPart;

		determineLatency(times);
		times_ms = determineMs(times);
		result_latency = getResult(times_ms);
		show(times_ms, "ms");

		show(result_latency, "ms");

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

