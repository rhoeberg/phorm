#pragma once

void MidiCCOperation(Node *self)
{
	i32 channel = self->params[0].i;
	i32 cc = self->params[1].i;

	i32 *out = GetInts()->Get(self->GetDataLast());
	*out = GetMidiCC(channel, cc);

	self->changed = true;
}

ObjectHandle CreateMidiCCNode()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("channel", 0),
		NodeParameter("cc", 0),
	};

	FixedArray<NodeInput> inputs = {
	};
	
	return AddNode(DATA_INT, params, inputs);
}
