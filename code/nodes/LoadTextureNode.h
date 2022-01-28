#pragma once

void LoadTextureOp(Node *self)
{
	int width;
	int height;
	int nrChannels;
	String *path = GetStrings()->Get(self->params[0].dataHandle);
	unsigned char *data = stbi_load(path->buffer, &width, &height, &nrChannels, STBI_rgb_alpha);
	if(data) {
		Texture *output = GetTextures()->Get(self->GetDataLast());
		output->Create(width, height);
		printf("width:%d\n", width);
		printf("height:%d\n", height);
		printf("nrChannels:%d\n", nrChannels);
		memcpy(output->pixels, data, sizeof(Pixel) * output->width * output->height);
		free(data);
	}
}

ObjectHandle LoadTextureCreate()
{
	printf("loadtexture create\n");
	FixedArray<NodeParameter> params = {
		NodeParameter("path", ""),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_TEXTURE),
	};

	return AddNode(DATA_TEXTURE, params, inputs);
}
