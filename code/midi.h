/*
  
  _midi port_
  channel amount: 16
  cc numbers: 127

 */

#pragma once

#define MIDI_CHANNEL_AMOUNT 16
#define MIDI_CC_AMOUNT 127

struct MidiChannel
{
	u8 cc[127];
};

/* struct MidiPort */
/* { */
	/* MidiChannel channels[16]; */
/* }; */

struct MidiState
{
	/* MidiPort port; */
	MidiChannel channels[16];
	std::vector<u8> messages;
	double delta;

	RtMidiIn *midiin;
};

MidiState *_midiState;

i32 GetMidiCC(u32 channel, u32 cc);
