#pragma once

int MixTextureOperation(Node *self);

int AddMixTextureNode()
{
	MixTextureNode mixNode = {};
	mixNode.mix = 0.5f;
	int typeHandle = _nodeState->mixTextureNodes.Insert(mixNode);
	int nodeHandle = AddNode(TEXTURE_NODE, typeHandle, MixTextureOperation);

	Node *node = GetNode(nodeHandle);
	if(node != NULL) {
		node->AddInput(TEXTURE_NODE);
		node->AddInput(TEXTURE_NODE);
	}

	return nodeHandle;
}

int MixTextureOperation(Node *self)
{
	// GET INPUTS
	Texture *tex1 = GetTextureInput(self->inputs[0]);
	Texture *tex2 = GetTextureInput(self->inputs[1]);
	if(!tex1 || !tex2)
		return -1;

	// GET SELF
	MixTextureNode *mixNode = &_nodeState->mixTextureNodes[self->typeHandle];
	Texture *output = GetTexture(self->dataHandle);

	// OPERATION
	float inputMul1 = 1.0f - mixNode->mix;
	float inputMul2 = mixNode->mix;
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

	return self->dataHandle;
}
