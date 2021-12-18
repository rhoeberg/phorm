#pragma once

void SinOperation(Node *self)
{
	double *input = GetDoubleOutput(&self->inputs[0].handle);
	if(!input)
		return;

	double *output = GetDouble(&self->GetDataLast());
	if(!output)
		return;

	*output = Sin(*input);
}

ObjectHandle CreateSinNode(String name, vec2 pos, DataType dataType, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_DOUBLE),
	};

	return AddNode(name.buffer, pos, dataType, op, drawFunc, params, inputs);
}
