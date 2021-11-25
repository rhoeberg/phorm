#pragma once

void MixTextureOperation(Node *self)
{
	// GET INPUTS
	Texture *tex1 = GetTextureInput(self->inputs[0]);
	Texture *tex2 = GetTextureInput(self->inputs[1]);
	if(!tex1 || !tex2)
		return;

	// GET SELF
	// TODO (rhoe) make cleaner way to get parameters
	double mix = self->params[0].d;
	Texture *output = GetTexture(self->GetDataLast());

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

int AddMixTextureNode()
{
	VMArray<NodeParameter> params = {
		NodeParameter("mix", PARAM_DOUBLE, 0.5),
	};

	VMArray<NodeInput> inputs = {
		NodeInput(TEXTURE_NODE),
		NodeInput(TEXTURE_NODE),
	};

	return AddNode("MIX_TEXTURE", TEXTURE_NODE, MixTextureOperation, params, inputs);
}
