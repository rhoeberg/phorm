#pragma once

Pixel* LoadTextureOperation(TextureNode *self);

struct LoadTextureNode {
	char path[128];
};

TextureNode* GetTextureNode(int handle)
{
	if(handle > ARRAY_SIZE(_nodeState->textureNodes) - 1 ||
	   handle < 0) {
		// invalid handle
		return NULL;
	}

	return &_nodeState->textureNodes[handle];
}

int AddLoadTextureNode()
{
	int nodeHandle = AddTextureNode();
	LoadTextureNode loadTextureNode = {};
	int dataHandle = _nodeState->loadTextureNodes.Insert(loadTextureNode);

	TextureNode *textureNode = &_nodeState->textureNodes[nodeHandle];
	textureNode->dataHandle = dataHandle;
	textureNode->GetPixels = LoadTextureOperation;

	return nodeHandle;
}

Pixel* LoadTextureOperation(TextureNode *self)
{
	if(self->changed) {
		int width;
		int height;
		int nrChannels;
		unsigned char *data = stbi_load("assets/pica.png", &width, &height, &nrChannels, STBI_rgb_alpha);
		if(data) {
			printf("width:%d\n", width);
			printf("height:%d\n", height);
			printf("nrChannels:%d\n", nrChannels);
			memcpy(self->pixels, data, sizeof(self->pixels));
			free(data);
		}

		self->changed = false;
	}

	return self->pixels;
}
