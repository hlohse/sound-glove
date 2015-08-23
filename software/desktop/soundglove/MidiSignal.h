/*      MidiSignal
 *
 *  Provides functionality to describe MIDI signals. Controls range from key
 *  and velocity to program presets etc.
 *  The resulting signal bytes can be retrieved via Signal and SignalLength.
 *
 *  Example, create middle C on channel 1
 *
 *      MidiSignal midi_signal;
 *
 *      midi_signal.Channel(1);
 *      midi_signal.Status(Midi::NoteOn);
 *      midi_signal.Key(60); // Middle C
 *      midi_signal.Velocity(63);
 */

#pragma once

#include "Midi.h"
#include "Export.h"
#include <string>

class MidiSignal {
public:
	SOUNDGLOVE_API MidiSignal();
	SOUNDGLOVE_API ~MidiSignal();

    // Currently: ControllerChange, default: Key/Velocity
	SOUNDGLOVE_API MidiSignal(const Midi::Status status,
                              const int channel,
                              const int value_1,
                              const int value_2);

    // Currently: default: PitchBend
	SOUNDGLOVE_API MidiSignal(const Midi::Status status,
                              const int channel,
                              const int value);

	SOUNDGLOVE_API char* Signal();
	SOUNDGLOVE_API int SignalLength() const;

	SOUNDGLOVE_API void Status(const Midi::Status status);
	SOUNDGLOVE_API Midi::Status Status() const;
    
	SOUNDGLOVE_API void Channel(const int channel);
	SOUNDGLOVE_API int Channel() const;

	SOUNDGLOVE_API void Key(const int key);
	SOUNDGLOVE_API int Key() const;

	SOUNDGLOVE_API void Velocity(const int velocity);
	SOUNDGLOVE_API int Velocity() const;

	SOUNDGLOVE_API void Pressure(const int pressure);
	SOUNDGLOVE_API int Pressure() const;

	SOUNDGLOVE_API void Controller(const Midi::Controller controller);
	SOUNDGLOVE_API Midi::Controller Controller() const;

	SOUNDGLOVE_API void ControllerValue(const int value);
	SOUNDGLOVE_API int ControllerValue() const;

	SOUNDGLOVE_API void Program(const int preset);
	SOUNDGLOVE_API int Program() const;

	SOUNDGLOVE_API void ChannelPressure(const int channel_pressure);
	SOUNDGLOVE_API int ChannelPressure() const;
    
	SOUNDGLOVE_API void PitchBend(const int pitch_bend);
	SOUNDGLOVE_API int PitchBend() const;

	SOUNDGLOVE_API std::string ToString() const;

private:
    uint8_t m_data[3];
    int m_data_length;

    void Value(const int value, const int m_dataindex);
    int  Value(const int m_dataindex) const;
};
