#pragma once

#include "MidiSignal.h"
#include "Export.h"
#include <functional>

class MIDIController {
public:
	SOUNDGLOVE_API MIDIController(const int controller_number,
		                          std::function<int(void)> controller_value);

	SOUNDGLOVE_API MidiSignal getMidiSignal() const;

private:
	const int controller_number_;
	std::function<int(void)> controller_value_;
};
