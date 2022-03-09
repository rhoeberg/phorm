#include "midi.h"

void MidiInputCallback(double deltaTime, std::vector<u8> *message, void *userData)
{
	u8 ccMessage = 0xB0;

	for(i32 i = 0; i < message->size(); i++) {
		printf("CALLBACK midi msg: %02x", message->at(i));

		if((message->at(i) & 0xF0) == ccMessage) {
			printf("\ttype: CC");
			i32 channel = (message->at(i) & 0x0F);
			printf("\tchannel: %d", channel);
			i++;
			u8 cc = message->at(i);
			printf("\tcc: %d", cc);
			i++;
			u8 value = message->at(i);
			printf("\tvalue: %d", value);

			if(channel < ARRAY_SIZE(_midiState->channels) - 1) {
				_midiState->channels[channel].cc[cc] = value;
			}
		}
		printf("\n");
	}
}

void InitializeMidi()
{
	ConsoleLog("MIDI: Initializing");

	_midiState = (MidiState*)calloc(1, sizeof(MidiState));

	_midiState->midiin = new RtMidiIn();

	u32 nPorts = _midiState->midiin->getPortCount();
	if(nPorts == 0) {
		ErrorLog("MIDI: could not find any midi ports\n");
	}
	else {
		DebugLog("MIDI: connecting to midi port: %s\n", _midiState->midiin->getPortName(0).c_str());
		try {
			_midiState->midiin->openPort(0);
		}
		catch(RtMidiError &error) {
			ErrorLog("MIDI: %s", error.getMessage());
		}
	}

	_midiState->midiin->setCallback(&MidiInputCallback);
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
