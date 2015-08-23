#include "MIDIController.h"

MIDIController::MIDIController(
	const int controller_number,
	std::function<int(void)> controller_value)
:	controller_number_(controller_number),
	controller_value_(controller_value)
{
}

MidiSignal MIDIController::getMidiSignal() const
{
	return{ Midi::Status::ControllerChange,
			1,
			controller_number_,
			controller_value_()
	};
}
