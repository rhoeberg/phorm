#pragma once

void OutputNodeOp(Node *self)
{
	Bitmap *input = GetBitmaps()->Get(self->inputs[0]);
	if(!input) {
		ErrorLog("Output node: invalid input texture\n");
		return;
	}

	/* Texture *output = GetTextures()->Get(self->GetDataLast()); */
	/* if(!input) { */
	/* 	ErrorLog("Output node: invalid output texture\n"); */
	/* 	return; */
	/* } */
	/* output->Create(input->width, input->height); */
	/* memcpy(output->pixels, input->pixels, sizeof(Pixel) * input->width * input->height); */
}

ObjectHandle CreateOutputNode()
{
	FixedArray<NodeParameter> params = {
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_BITMAP),
	};

	return AddNode(DATA_NONE, params, inputs);
}
