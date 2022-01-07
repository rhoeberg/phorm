#pragma once

void MixTextureOperation(Node *self)
{
	// GET INPUTS
	Texture *tex1 = GetTextures()->Get(self->inputs[0]);
	Texture *tex2 = GetTextures()->Get(self->inputs[1]);
	if(!tex1 || !tex2)
		return;

	// GET SELF
	// TODO (rhoe) make cleaner way to get parameters
	double mix = self->params[0].Double();
	Texture *output = GetTextures()->Get(self->GetDataLast());

	// OPERATION
	float inputMul1 = 1.0f - mix;
	float inputMul2 = mix;
	for(int x = 0; x < TEXTURE_SIZE; x++) {
		for(int y = 0; y < TEXTURE_SIZE; y++) {
			int index = GetPixelIndex(x, y);
			Pixel result = {};
			result.r = ((tex1->pixels[index].r * inputMul1) + (tex2->pixels[index].r * inputMul2));
			result.g = ((tex1->pixels[index].g * inputMul1) + (tex2->pixels[index].g * inputMul2));
			result.b = ((tex1->pixels[index].b * inputMul1) + (tex2->pixels[index].b * inputMul2));
			output->pixels[index] = result;
		}
	}
}

ObjectHandle CreateMixTexture(String name, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
		NodeParameter("mix", 0.5),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_TEXTURE),
		NodeInput(DATA_TEXTURE),
	};

	return AddNode(name.buffer, DATA_TEXTURE, op, drawFunc, params, inputs);
}
