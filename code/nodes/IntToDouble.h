#pragma once

void I2DOp(Node *self)
{
	double *out = GetDoubles()->Get(self->GetDataLast());
	*out = (double)*GetInts()->Get(self->inputs[0]);
}

ObjectHandle CreateI2DNode()
{
	FixedArray<NodeParameter> params = {
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_INT),
	};

	return AddNode(DATA_DOUBLE, params, inputs);
}
