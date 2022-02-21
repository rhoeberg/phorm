#pragma once

/*
  This node mixes two textures
  
  for now only does a simple stretch algorithm using truncation to handle different 
  size textures.
  
  the order parameter decides which texture is used as the final output size. 
  0 = the first texture (default)
  not 0 = the second texture 
  
 */
void MixTextureOperation(Node *self)
{
	// GET PARAMETERS
	double mix = self->params[0].Double();
	i32 order = self->params[1].Int();

	// GET INPUTS
	Bitmap *input1 = NULL;
	Bitmap *input2 = NULL;
	if(order == 0) {
		input1 = GetBitmaps()->Get(self->inputs[0]);
		input2 = GetBitmaps()->Get(self->inputs[1]);
	}
	else {
		input1 = GetBitmaps()->Get(self->inputs[1]);
		input2 = GetBitmaps()->Get(self->inputs[0]);
	}

	if(!input1 || !input2) {
		ErrorLog("MixTextureNode: could not find inputs");
		return;
	}

	Bitmap *output = GetBitmaps()->Get(self->GetDataLast());
	if(!output) {
		ErrorLog("MixTextureNode: could not find output");
		return;
	}

	output->Create(input1->width, input1->height);

	// OPERATION
	float inputMul1 = 1.0f - mix;
	float inputMul2 = mix;
	float stepX = (float)input2->width / (float)input1->width;
	float stepY = (float)input2->height / (float)input1->height;
	for(int x = 0; x < output->width; x++) {
		for(i32 y = 0; y < output->height; y++) {
			i32 index1 = GetPixelIndex(x, y, input1->width);

			i32 input2X = (i32)(x * stepX);
			i32 input2Y = (i32)(y * stepY);

			i32 index2 = GetPixelIndex(input2X, input2Y, input2->width);
			Pixel result = {};

			result.r = ((input1->pixels[index1].r * inputMul1) + (input2->pixels[index2].r * inputMul2));
			result.g = ((input1->pixels[index1].g * inputMul1) + (input2->pixels[index2].g * inputMul2));
			result.b = ((input1->pixels[index1].b * inputMul1) + (input2->pixels[index2].b * inputMul2));

			output->pixels[index1] = result;
		}
	}
}

ObjectHandle CreateMixTexture()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("mix", 0.5),
		NodeParameter("order", 0),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_BITMAP),
		NodeInput(DATA_BITMAP),
	};

	return AddNode(DATA_BITMAP, params, inputs);
}
