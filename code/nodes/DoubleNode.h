#pragma once

void DoubleOperation(Node *self)
{
	double *output = GetDoubles()->Get(self->GetDataLast());
	if(!output)
		return;

	*output = self->params[0].Double();
}

ObjectHandle CreateDoubleNode()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("value", 0.0),
	};

	FixedArray<NodeInput> inputs = {
	};

	return AddNode(DATA_DOUBLE, params, inputs);
}
