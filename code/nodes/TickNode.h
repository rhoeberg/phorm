#pragma once

void TickNodeOp(Node *self)
{
	i32 *output  = GetInts()->Get(self->GetDataLast());

	if(((i32)(GetTime() * self->params[0].Double()) % 2) == 1 && *output != 1) {
		self->changed = true;
		*output = 1;
	}
	else if (((i32)(GetTime() * self->params[0].Double()) % 2) == 0) {
		self->changed = false;
		*output = 0;
	}
	else {
		self->changed = false;
	}
}

ObjectHandle CreateTickNode()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("rate", 1.0),
	};

	FixedArray<NodeInput> inputs = {
	};

	return AddNode(DATA_INT, params, inputs);
}
