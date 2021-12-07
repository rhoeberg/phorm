#pragma once

void RenderObjectOperation(Node *self)
{
	RenderObject *output = GetRenderObject(self->GetDataLast());

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


    /* glBindVertexArray(output->VAO); */
	/* glBind */

    glBindBuffer(GL_ARRAY_BUFFER, output->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * inputMesh->vertices.Count(), inputMesh->vertices.Data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, output->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * inputMesh->indices.Count(), inputMesh->indices.Data(), GL_STATIC_DRAW);

    /* glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0); */
    /* glEnableVertexAttribArray(0); */
    
    /* glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat))); */
    /* glEnableVertexAttribArray(1); */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /* glBindVertexArray(0); */

	BindBuffersToVAO(output->VAOHandle, output->VBO, output->EBO);
	BindBuffersToVAO(output->VAOHandle, output->VBO, output->EBO);
	/* BindBufferToVAO(output->VAOHandle, GL_ARRAY_BUFFER, output->VBO); */
	/* BindBufferToVAO(output->VAOHandle, GL_ELEMENT_ARRAY_BUFFER, output->EBO); */
	/* SetVAOAttribPointers(output->VAOHandle); */

	output->indicesCount = inputMesh->indices.Count();
}

int AddRenderObject()
{
	VMArray<NodeParameter> params = {
	};

	VMArray<NodeInput> inputs = {
		NodeInput(DATA_MESH),
		NodeInput(DATA_TEXTURE),
	};

	return AddNode("RENDER_OBJECT", DATA_RENDEROBJECT, RenderObjectOperation, params, inputs);
}
