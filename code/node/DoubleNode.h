#pragma once

void DoubleOperation(Node *self)
{
	double *output = GetDouble(&self->GetDataLast());
	if(!output)
		return;

	*output = self->params[0].Double();
}

ObjectHandle CreateDoubleNode(String name, vec2 pos, DataType dataType, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
		NodeParameter("value", 0.0),
	};

	FixedArray<NodeInput> inputs = {
	};

	return AddNode(name.buffer, pos, dataType, op, drawFunc, params, inputs);
}
