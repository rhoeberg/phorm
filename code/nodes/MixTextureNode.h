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
	Texture *tex1 = NULL;
	Texture *tex2 = NULL;
	if(order == 0) {
		tex1 = GetTextures()->Get(self->inputs[0]);
		tex2 = GetTextures()->Get(self->inputs[1]);
	}
	else {
		tex1 = GetTextures()->Get(self->inputs[1]);
		tex2 = GetTextures()->Get(self->inputs[0]);
	}

	if(!tex1 || !tex2) {
		ErrorLog("MixTextureNode: could not find inputs");
		return;
	}

	Texture *output = GetTextures()->Get(self->GetDataLast());
	if(!output) {
		ErrorLog("MixTextureNode: could not find output");
		return;
	}

	output->Create(tex1->width, tex1->height);

	// OPERATION
	float inputMul1 = 1.0f - mix;
	float inputMul2 = mix;
	float stepX = (float)tex2->width / (float)tex1->width;
	float stepY = (float)tex2->height / (float)tex1->height;
	for(int x = 0; x < output->width; x++) {
		for(i32 y = 0; y < output->height; y++) {
			i32 index1 = GetPixelIndex(x, y, tex1->width);

			i32 tex2X = (i32)(x * stepX);
			i32 tex2Y = (i32)(y * stepY);

			i32 index2 = GetPixelIndex(tex2X, tex2Y, tex2->width);
			Pixel result = {};

			/* if(x > (tex2->width - 1) || */
			/*    y > (tex2->height - 1)) { */
			/* 	result = tex1->pixels[index1]; */
			/* } */
			/* else { */
			result.r = ((tex1->pixels[index1].r * inputMul1) + (tex2->pixels[index2].r * inputMul2));
			result.g = ((tex1->pixels[index1].g * inputMul1) + (tex2->pixels[index2].g * inputMul2));
			result.b = ((tex1->pixels[index1].b * inputMul1) + (tex2->pixels[index2].b * inputMul2));
			/* } */

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
		NodeInput(DATA_TEXTURE),
		NodeInput(DATA_TEXTURE),
	};

	return AddNode(DATA_TEXTURE, params, inputs);
}
