#pragma once

void RandIntOp(Node *self)
{
	int *output = GetInts()->Get(self->GetDataLast());
	if(!output) {
		ErrorLog("RandIntNode: can not find output");
	}

	int min = self->params[0].Int();
	int max = self->params[1].Int();

	*output = RandInt(min, max);
}

ObjectHandle CreateRandIntNode()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("min", 0),
		NodeParameter("max", 10),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_INT),
	};

	return AddNode(DATA_INT, params, inputs);
}
