#pragma once

void LoadTextureOp(Node *self)
{
	i32 width;
	i32 height;
	i32 nrChannels;
	String *path = GetStrings()->Get(self->params[0].dataHandle);
	unsigned char *data = stbi_load(path->buffer, &width, &height, &nrChannels, STBI_rgb_alpha);
	if(data) {
		Bitmap *output = GetBitmaps()->Get(self->GetDataLast());
		output->Create(width, height);
		memcpy(output->pixels, data, sizeof(Pixel) * output->width * output->height);
		free(data);
	}
	else {
		WarningLog("problem loading texture: %s", path->buffer);
	}
}

ObjectHandle LoadTextureCreate()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("path", ""),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_BITMAP),
	};

	return AddNode(DATA_BITMAP, params, inputs);
}
