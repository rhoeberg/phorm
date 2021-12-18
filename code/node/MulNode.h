#pragma once

void MulOperation(Node *self)
{
	double *input1 = GetDoubleOutput(&self->inputs[0].handle);
	if(!input1)
		return;

	double *input2 = GetDoubleOutput(&self->inputs[1].handle);
	if(!input2)
		return;

	double *output = GetDouble(&self->GetDataLast());
	*output = *input1 * *input2;

}

ObjectHandle CreateMulNode(String name, vec2 pos, DataType dataType, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_DOUBLE),
		NodeInput(DATA_DOUBLE),
	};

	return AddNode(name.buffer, pos, dataType, op, drawFunc, params, inputs);
}
