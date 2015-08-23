#include "MIDINote.h"
#include "Midi.h"
using namespace std;

MIDINote::MIDINote(
	const std::function<int(void)>  key,
	const std::function<int(void)>  velocity)
:	key_(key),
	velocity_(velocity),
	previous_midi_signal_({ Midi::Status::NoteOff, 0, 0, 0 })
{
}

bool MIDINote::changed() const
{
	const MidiSignal new_midi_signal = {
		Midi::Status::NoteOn,
		1,
		key_(),
		velocity_()
	};
	return new_midi_signal.Key()      != previous_midi_signal_.Key()
		|| new_midi_signal.Velocity() != previous_midi_signal_.Velocity();
}

array<MidiSignal, 2> MIDINote::getMidiSignals()
{
	const MidiSignal new_midi_signal = {
		Midi::Status::NoteOn,
		1,
		key_(),
		velocity_()
	};
	const array<MidiSignal, 2> midi_signals = { {
		{	Midi::Status::NoteOff,
			previous_midi_signal_.Channel(),
			previous_midi_signal_.Key(),
			previous_midi_signal_.Velocity()
		},
		new_midi_signal
	} };
	previous_midi_signal_ = new_midi_signal;
	return midi_signals;
}
