// TODO (rhoe) passing shader as arg is a tmp hack
//             will be replaced once we have materials
void RenderObjectInstance::Render(Shader *shader)
{
	RenderObject *renderObject = GetRenderObjects()->Get(renderObjectHandle);
	if(!renderObject) {
		DebugLog("SceneObject: tried to render sceneobject without renderobject");
		return;
	}

	///////////////////
	// WIREFRAME MODE
	// if(renderObject->primitiveMode == G) {
	// 	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// }
	// else {
	// 	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// }

	// TODO (rhoe) change default shader to material shader attached to renderobject
	shader->Begin();

	glActiveTexture(GL_TEXTURE0);
	if(renderObject->hasTexture)
		GFXTextureBind(renderObject->textureHandle);
	else
		GFXBindDefaultTexture();


	mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	model = glm::translate(model, renderObject->pos);
	model = glm::translate(model, pos);
	quat q = quat(renderObject->rot) * quat(rot);
	glm::mat4 rotationMatrix = glm::mat4_cast(q);
	model = model * rotationMatrix;
	model = glm::scale(model, renderObject->scale * scale);

	glm::mat3 model3x3 = glm::mat3(model);
	glm::mat3 normalMatrix = glm::inverseTranspose(model3x3);
	shader->SetUniform("normalMatrix", normalMatrix);
	shader->SetUniform("model", model);

	// set color
	shader->SetUniform("objectColor", renderObject->color);

	/////////////////
	// DRAW
	GLuint VAO = GetCurrentContextVAO(renderObject->VAOHandle);
	glBindVertexArray(VAO);

	GFXPointSize(renderObject->pointSize);
	if(renderObject->useIndices) {
		GFXDraw(renderObject->VAOHandle, renderObject->primitiveMode, renderObject->indexCount, true);
	}
	else {
		GFXDraw(renderObject->VAOHandle, renderObject->primitiveMode, renderObject->vertexCount, false);
	}
		 

	glBindVertexArray(0);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	shader->End();
}
