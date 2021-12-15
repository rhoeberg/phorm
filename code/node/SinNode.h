#pragma once

void SinOperation(Node *self)
{
	double *input = GetDouble(&self->inputs[0].handle);
	if(!input)
		return;

	double *output = GetDouble(&self->GetDataLast());
	if(!output)
		return;

	*output = Sin(*input);
}

/* ObjectHandle AddSinNode() */
/* { */
/* 	FixedArray<NodeParameter> params = { */
/* 	}; */
/* 	FixedArray<NodeInput> inputs = { */
/* 		NodeInput(DATA_DOUBLE), */
/* 	}; */

/* 	return AddNode("SIN", DATA_DOUBLE, OP_DOUBLE_SIN, params, inputs); */
/* } */
