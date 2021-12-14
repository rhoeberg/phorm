#pragma once

void LoadTextureOperation(Node *self)
{
	int width;
	int height;
	int nrChannels;
	unsigned char *data = stbi_load("assets/pica.png", &width, &height, &nrChannels, STBI_rgb_alpha);
	if(data) {
		Texture *output = GetTexture(&self->GetDataLast());
		printf("width:%d\n", width);
		printf("height:%d\n", height);
		printf("nrChannels:%d\n", nrChannels);
		memcpy(output->pixels, data, sizeof(output->pixels));
		free(data);
	}
}

ObjectHandle AddLoadTextureNode()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("path", ""),
	};

	FixedArray<NodeInput> inputs = {
	};

	return AddNode("LOAD_TEXTURE", DATA_TEXTURE, OP_TEXTURE_LOAD, params, inputs);
}
