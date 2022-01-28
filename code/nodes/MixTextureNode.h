#pragma once

void MixTextureOperation(Node *self)
{
	// GET INPUTS
	Texture *tex1 = GetTextures()->Get(self->inputs[0]);
	Texture *tex2 = GetTextures()->Get(self->inputs[1]);
	if(!tex1 || !tex2)
		return;

	if(tex1->width != tex2->width && 
	   tex1->height != tex2->height) {
		DebugLog("Mix Texture cannot use two different size textures\n");
		return;
	}

	// GET SELF
	// TODO (rhoe) make cleaner way to get parameters
	double mix = self->params[0].Double();
	Texture *output = GetTextures()->Get(self->GetDataLast());
	output->Create(tex1->width, tex1->height);

	// OPERATION
	float inputMul1 = 1.0f - mix;
	float inputMul2 = mix;
	for(int x = 0; x < output->width; x++) {
		for(int y = 0; y < output->height; y++) {
			int index = GetPixelIndex(x, y, output->height);
			Pixel result = {};
			result.r = ((tex1->pixels[index].r * inputMul1) + (tex2->pixels[index].r * inputMul2));
			result.g = ((tex1->pixels[index].g * inputMul1) + (tex2->pixels[index].g * inputMul2));
			result.b = ((tex1->pixels[index].b * inputMul1) + (tex2->pixels[index].b * inputMul2));
			output->pixels[index] = result;
		}
	}
}

ObjectHandle CreateMixTexture()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("mix", 0.5),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_TEXTURE),
		NodeInput(DATA_TEXTURE),
	};

	return AddNode(DATA_TEXTURE, params, inputs);
}
