#pragma once

#include "Export.h"
#include <windows.h>

class Timer {
public:
	// Call once first!
	SOUNDGLOVE_API static void initialize();

	SOUNDGLOVE_API Timer();

	SOUNDGLOVE_API void start();
	SOUNDGLOVE_API void stop();

	SOUNDGLOVE_API double duration() const; // [s]

private:
	static double performance_counter_frequency; // [Hz]

	LARGE_INTEGER start_;
	LARGE_INTEGER stop_;
};
