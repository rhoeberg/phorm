#pragma once

void CubeOperation(Node *self)
{
	RenderObject *output = GetRenderObject(self->GetDataLast());
	
	Texture *inputTexture = GetTextureInput(self->inputs[0]);
	if(inputTexture) {
		glBindTexture(GL_TEXTURE_2D, output->textureID);
		// TODO (rhoe) define texture width and height in texture object
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, inputTexture->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		output->hasTexture = true;
	}
	else {
		output->hasTexture = false;
	}

	// TODO (rhoe) create opengl ids on init
	output->VAO = createCubeVAO();
	output->vertexAmount = 36;
}

int AddCubeNode()
{
	VMArray<NodeParameter> params = {
	};

	VMArray<NodeInput> inputs = {
		NodeInput(TEXTURE_NODE),
	};

	return AddNode("CUBE", RENDEROBJECT_NODE, CubeOperation, params, inputs);
}

