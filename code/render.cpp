#include "render.h"

void InitializeRender()
{
	_renderState = (RenderState*)malloc(sizeof(RenderState));

	// TODO (rhoe) move this stuff to a rendering system
    glm::mat4 view = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
    // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.2f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// setup shaders
    _renderState->baseShader = createShaderProgram("assets/base.vert", "assets/base.frag");
    glUseProgram(_renderState->baseShader);
    GLuint viewLoc = glGetUniformLocation(_renderState->baseShader, "view");
    GLuint projectionLoc = glGetUniformLocation(_renderState->baseShader, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    _renderState->textureShader = createShaderProgram("assets/texture.vert", "assets/texture.frag");
    glUseProgram(_renderState->textureShader);
    viewLoc = glGetUniformLocation(_renderState->textureShader, "view");
    projectionLoc = glGetUniformLocation(_renderState->textureShader, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	_renderState->cubeVAO = createCubeVAO();
}

void RenderCubeTexture_TMP(vec3 pos, GLuint texture)
{
	glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	model = glm::translate(model, pos);
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0, 1, 0));

	// render with texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUseProgram(_renderState->textureShader);
	GLuint modelLoc = glGetUniformLocation(_renderState->textureShader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	
	// render cube
	glBindVertexArray(_renderState->cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glUseProgram(0);
}

void RenderCubeBase_TMP(vec3 pos)
{
	glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	model = glm::translate(model, pos);
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0, 1, 0));
	// render without texture
	glUseProgram(_renderState->baseShader);
	GLuint modelLoc = glGetUniformLocation(_renderState->baseShader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// render cube
	glBindVertexArray(_renderState->cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glUseProgram(0);
}

GLuint GetTextureShader()
{
	return _renderState->textureShader;
}

void UpdateRender()
{
	///////////////
	//RENDER
	///////////////
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
}

void CleanupRender()
{
	free(_renderState);
}
