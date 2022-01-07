#pragma once

void DoubleOperation(Node *self)
{
	double *output = GetDoubles()->Get(self->GetDataLast());
	if(!output)
		return;

	*output = self->params[0].Double();
}

ObjectHandle CreateDoubleNode(String name, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
		NodeParameter("value", 0.0),
	};

	FixedArray<NodeInput> inputs = {
	};

	return AddNode(name.buffer, DATA_DOUBLE, op, drawFunc, params, inputs);
}
