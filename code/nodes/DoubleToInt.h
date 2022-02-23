#pragma once

void D2IOp(Node *self)
{
	int *out = GetInts()->Get(self->GetDataLast());
	*out = (int)*GetDoubles()->Get(self->inputs[0]);
}

ObjectHandle CreateD2INode()
{
	FixedArray<NodeParameter> params = {
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_DOUBLE),
	};

	return AddNode(DATA_INT, params, inputs);
}
