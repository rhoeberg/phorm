#pragma once

void LoadTextureOp(Node *self)
{
	int width;
	int height;
	int nrChannels;
	unsigned char *data = stbi_load("assets/pica.png", &width, &height, &nrChannels, STBI_rgb_alpha);
	if(data) {
		Texture *output = GetTextures()->Get(self->GetDataLast());
		printf("width:%d\n", width);
		printf("height:%d\n", height);
		printf("nrChannels:%d\n", nrChannels);
		memcpy(output->pixels, data, sizeof(output->pixels));
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
