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
