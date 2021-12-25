#pragma once

void RenderObjectOperation(Node *self)
{
	RenderObject *output = GetRenderObject(&self->GetDataLast());

	output->pos = self->params[0].Vec3();
	output->scale = self->params[1].Vec3();

	Mesh *inputMesh = GetMeshInput(self->inputs[0]);
	if(!inputMesh)
		return;
	
	Texture *inputTexture = GetTextureInput(self->inputs[1]);
	if(inputTexture) {
		glBindTexture(GL_TEXTURE_2D, output->textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, inputTexture->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		output->hasTexture = true;
	}
	else {
		output->hasTexture = false;
	}

    glBindBuffer(GL_ARRAY_BUFFER, output->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * inputMesh->vertices.Count(), inputMesh->vertices.Data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, output->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * inputMesh->indices.Count(), inputMesh->indices.Data(), GL_STATIC_DRAW);

	BindBuffersToVAO(output->VAOHandle, output->VBO, output->EBO);

	output->indicesCount = inputMesh->indices.Count();
}

ObjectHandle CreateRenderObjectNode(String name, vec2 pos, DataType dataType, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
		NodeParameter("pos", vec3(0, 0, 0)),
		NodeParameter("scale", vec3(1.0f, 1.0f, 1.0f)),
		NodeParameter("label", ""),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_MESH),
		NodeInput(DATA_TEXTURE),
	};

	return AddNode(name.buffer, pos, dataType, op, drawFunc, params, inputs);
}
