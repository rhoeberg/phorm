#pragma once

int LoadTextureOperation(Node *self);

int AddLoadTextureNode()
{
	LoadTextureNode loadTextureNode = {};
	int typeHandle = _nodeState->loadTextureNodes.Insert(loadTextureNode);
	int nodeHandle = AddNode("LOAD_TEXTURE", TEXTURE_NODE, typeHandle, LoadTextureOperation);
	return nodeHandle;
}

int LoadTextureOperation(Node *self)
{
	int width;
	int height;
	int nrChannels;
	unsigned char *data = stbi_load("assets/pica.png", &width, &height, &nrChannels, STBI_rgb_alpha);
	if(data) {
		Texture *output = GetTexture(self->GetDataLast());
		printf("width:%d\n", width);
		printf("height:%d\n", height);
		printf("nrChannels:%d\n", nrChannels);
		memcpy(output->pixels, data, sizeof(output->pixels));
		free(data);
	}

	return self->GetDataLast();
}
