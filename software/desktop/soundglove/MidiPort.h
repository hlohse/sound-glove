#pragma once

#include "Export.h"
#include "MidiSignal.h"
#include "MIDINote.h"
#include "MIDIController.h"
#include <teVirtualMIDI.h>
#include <string>

class MidiPort {
public:
	SOUNDGLOVE_API  MidiPort(const std::string& name);
	SOUNDGLOVE_API ~MidiPort();

	SOUNDGLOVE_API void Play(MidiSignal& midi_signal);
	SOUNDGLOVE_API void Play(MIDIController& midi_controller);
	SOUNDGLOVE_API void Play(MIDINote& midi_note);

	SOUNDGLOVE_API std::string Name() const;

private:
    std::string    m_name;
	LPVM_MIDI_PORT m_port;

    static const int buffer_size = 0x1FFFE;
};
