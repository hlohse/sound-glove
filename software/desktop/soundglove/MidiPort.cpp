#include "MidiPort.h"
#include "Formatter.h"
#include <stdexcept>
#include <locale>
#include <codecvt>
using namespace std;

MidiPort::MidiPort(const std::string& name)
:   m_name(""),
    m_port(nullptr)
{
	wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
	const wstring m_nameutf8 = converter.from_bytes(name);
	const LPCWSTR m_portname = m_nameutf8.c_str();

	m_port = virtualMIDICreatePortEx(m_portname,
		NULL,
		0,
		MidiPort::buffer_size);

	if (m_port == nullptr) {
		throw runtime_error(Formatter()
			<< "Failed to open virtual MIDI port \"" << name << "\""
			<< ": " << GetLastError());
	}

	m_name = name;
}

MidiPort::~MidiPort()
{
	if (m_port != nullptr) {
		virtualMIDIClosePort(m_port);
		m_port = nullptr;
	}
}

void MidiPort::Play(MidiSignal& midi_signal)
{
    if (!virtualMIDISendData(m_port,
                             (LPBYTE) midi_signal.Signal(),
                             midi_signal.SignalLength()))
    {
        throw runtime_error(Formatter()
            << "Failed to play MIDI signal "
            << midi_signal.ToString()  << ": " << GetLastError());
    }
}

void MidiPort::Play(MIDIController& midi_controller)
{
	Play(midi_controller.getMidiSignal());
}

void MidiPort::Play(MIDINote& midi_note)
{
	if (midi_note.changed()) {
		for (MidiSignal& signal : midi_note.getMidiSignals()) {
			Play(signal);
		}
	}
}

string MidiPort::Name() const
{
    return m_name;
}
