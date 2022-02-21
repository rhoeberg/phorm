#pragma once

void RenderObjectOperation(Node *self)
{
	RenderObject *output = GetRenderObjects()->Get(self->GetDataLast());

	// parameters
	output->pos = self->params[0].Vec3();
	output->scale = self->params[1].Vec3();
	output->rot = self->params[2].Vec3();
	output->color = self->params[2].Vec4();

	Bitmap *inputBitmap = GetBitmaps()->Get(self->inputs[1]);
	if(inputBitmap) {
		GFXTextureUploadBitmap(output->textureHandle, inputBitmap);
		output->hasTexture = true;
	}
	else {
		output->hasTexture = false;
	}

	Mesh *inputMesh = GetMeshes()->Get(self->inputs[0]);
	if(!inputMesh)
		return;

    glBindBuffer(GL_ARRAY_BUFFER, output->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * inputMesh->vertices.Count(), inputMesh->vertices.Data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, output->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * inputMesh->indices.Count(), inputMesh->indices.Data(), GL_STATIC_DRAW);

	BindBuffersToVAO(output->VAOHandle, output->VBO, output->EBO);

	output->indicesCount = inputMesh->indices.Count();
}

ObjectHandle CreateRenderObjectNode()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("pos", vec3(0, 0, 0)),
		NodeParameter("scale", vec3(1.0f, 1.0f, 1.0f)),
		NodeParameter("rot", vec3(0.0f, 0.0f, 0.0f)),
		NodeParameter("color", vec4(1.0f, 1.0f, 1.0f, 1.0f)),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_MESH),
		NodeInput(DATA_BITMAP),
	};

	return AddNode(DATA_RENDEROBJECT, params, inputs);
}
