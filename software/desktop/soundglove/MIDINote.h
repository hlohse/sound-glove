#pragma once

#include "MidiSignal.h"
#include "Export.h"
#include <array>
#include <functional>

class MIDINote {
public:
	typedef int  (*key_f)();
	typedef int  (*velocity_f)();

	SOUNDGLOVE_API MIDINote(const std::function<int(void)>  key,
							const std::function<int(void)>  velocity);

	SOUNDGLOVE_API bool changed() const;
	SOUNDGLOVE_API std::array<MidiSignal, 2> getMidiSignals();

private:
	const std::function<int(void)>  key_;
	const std::function<int(void)>  velocity_;
	MidiSignal previous_midi_signal_;
};
