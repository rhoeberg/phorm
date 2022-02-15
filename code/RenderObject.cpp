// void RenderObject::Render()
// {
// 	// TODO (rhoe) change default shader to material shader attached to renderobject
// 	defaultShader.Use();

// 	glActiveTexture(GL_TEXTURE0);
// 	if(hasTexture)
// 		glBindTexture(GL_TEXTURE_2D, textureID);
// 	else
// 		glBindTexture(GL_TEXTURE_2D, defaultTexture);


// 	mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
// 	model = glm::translate(model, pos);
// 	quat q = quat(rot);
// 	glm::mat4 rotationMatrix = glm::mat4_cast(q);
// 	model = model * rotationMatrix;
// 	model = glm::scale(model, scale);

// 	glm::mat3 model3x3 = glm::mat3(model);
// 	glm::mat3 normalMatrix = glm::inverseTranspose(model3x3);
// 	defaultShader.SetUniform("normalMatrix", normalMatrix);

// 	// set color
// 	defaultShader.SetUniform("objectColor", vec3(1, 0, 0));
// 	defaultShader.SetUniform("model", model);

// 	/////////////////
// 	// DRAW
// 	GLuint VAO = GetCurrentContextVAO(VAOHandle);
// 	glBindVertexArray(VAO);
// 	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
// 	glBindVertexArray(0);
// }

