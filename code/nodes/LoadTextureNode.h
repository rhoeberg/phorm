#pragma once

void LoadTextureOp(Node *self)
{
	i32 width;
	i32 height;
	i32 nrChannels;
	String *path = GetStrings()->Get(self->params[0].dataHandle);
	unsigned char *data = stbi_load(path->buffer, &width, &height, &nrChannels, STBI_rgb_alpha);
	if(data) {
		Texture *output = GetTextures()->Get(self->GetDataLast());
		output->Create(width, height);
		DebugLog("width:%d\n", width);
		DebugLog("height:%d\n", height);
		DebugLog("nrChannels:%d\n", nrChannels);
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
