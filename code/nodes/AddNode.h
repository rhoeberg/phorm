#pragma once

void AddOperation(Node *self)
{
	double *input1 = GetDoubles()->Get(&self->inputs[0]);
	if(!input1)
		return;

	double *input2 = GetDoubles()->Get(&self->inputs[1]);
	if(!input2)
		return;

	double *output = GetDoubles()->Get(&self->GetDataLast());
	*output = *input1 + *input2;

}

ObjectHandle CreateAddNode(String name, vec2 pos, DataType dataType, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_DOUBLE),
		NodeInput(DATA_DOUBLE),
	};

	return AddNode(name.buffer, pos, dataType, op, drawFunc, params, inputs);
}
