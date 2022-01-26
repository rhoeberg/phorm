#include "midi.h"

void InitializeMidi()
{
	_midiState = (MidiState*)calloc(1, sizeof(MidiState));

	_midiState->midiin = new RtMidiIn();

	u32 nPorts = _midiState->midiin->getPortCount();
	if(nPorts == 0) {
		ErrorLog("MIDI: could not find any midi ports\n");
		// exit(0);
	}
	else {
		DebugLog("MIDI: connecting to midi port: %s\n", _midiState->midiin->getPortName(0).c_str());
		_midiState->midiin->openPort(0);
	}
}

void MidiReadMessages()
{
	u8 ccMessage = 0xB0;

	_midiState->delta = _midiState->midiin->getMessage(&_midiState->messages);

	for(i32 i = 0; i < _midiState->messages.size(); i++) {
		printf("midi msg: %02x", _midiState->messages[i]);

		if((_midiState->messages[i] & 0xF0) == ccMessage) {
			printf("\ttype: CC");
			i32 channel = (_midiState->messages[i] & 0x0F);
			printf("\tchannel: %d", channel);
			i++;
			u8 cc = _midiState->messages[i];
			printf("\tcc: %d", cc);
			i++;
			u8 value = _midiState->messages[i];
			printf("\tvalue: %d", value);

			if(channel < ARRAY_SIZE(_midiState->channels) - 1) {
				_midiState->channels[channel].cc[cc] = value;
			}
		}

		printf("\n");
	}
}

void UpdateMidi()
{
	MidiReadMessages();
}

i32 GetMidiCC(u32 channel, u32 cc)
{
	if(channel < MIDI_CHANNEL_AMOUNT && 
	   cc < MIDI_CC_AMOUNT) {
		return _midiState->channels[channel].cc[cc];
	}

	// if invalid midi channel or cc return 0
	return 0;
}

void CleanupMidi()
{
	delete _midiState->midiin;
}
