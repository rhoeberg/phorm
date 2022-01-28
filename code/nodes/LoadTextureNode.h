#pragma once

void LoadTextureOp(Node *self)
{
	int width;
	int height;
	int nrChannels;
	String *path = GetStrings()->Get(self->params[0].dataHandle);
	/* unsigned char *data = stbi_load("assets/picahigh.jpg", &width, &height, &nrChannels, STBI_rgb_alpha); */
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

/* // this needs to happen after node has been somehow created */
/* // as new or loaded */
/* void LoadTextureSetup(Node *self) */
/* { */
/* 	// setup OP */
/* 	// setup DrawFunc */
/* 	// setup edit func */
/* 	// create data and add handles */

/* 	self->op = LoadTextureOperation; */
/* 	self->drawFunc = BaseNodeDrawFunc; */
/* 	self->SetDataHandle(_nodeState->textures.InsertNew()); */
/* } */

/* ObjectHandle LoadTextureCreate(String name, NodeOp op, NodeDrawFunc drawFunc) */
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
