#include "viewer_render.h"

void InitializeViewerRender()
{
	_viewerRenderState.baseTextureObject = AddNewRenderObject();
	RenderObject *renderObject = GetRenderObject(_viewerRenderState.baseTextureObject);

	// TODO (rhoe) currently we are wasting a gl id here
	//             perhaps we should pass vao as parameter instead of
	//             creating a new one
	renderObject->VAOHandle = CreateSpriteVAO();
	renderObject->hasTexture = true;
	renderObject->indicesCount = 12;
}

void AddTextureToRenderQueue(DataHandle textureHandle)
{
	if(textureHandle.type != TEXTURE_NODE) {
		// TODO err
	}
	else {
		RenderObject *renderObject = GetRenderObject(_viewerRenderState.baseTextureObject);
		Texture *texture = GetTexture(textureHandle);

		glBindTexture(GL_TEXTURE_2D, renderObject->textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		AddToRenderQueue(_viewerRenderState.baseTextureObject);
	}
}

void AddToRenderQueue(DataHandle objectHandle)
{
	if(objectHandle.type != RENDEROBJECT_NODE) {
		// TODO err
	}
	else {
		_viewerRenderState.renderList.Insert(objectHandle);
	}
}

void UpdateViewerRender()
{
	///////////////////
	// SETUP VIEWER MODE
	float aspectRatio;
	if(ViewerInMain()) {
		glfwMakeContextCurrent(_win);
		int width, height;
		GetWindowSize(&width, &height);
		aspectRatio = (float)VIEWER_SIZE / (float)VIEWER_SIZE;
		glViewport(width - VIEWER_SIZE, height - VIEWER_SIZE, VIEWER_SIZE, VIEWER_SIZE);
	}
	else {
		glfwMakeContextCurrent(_viewerWindow);
		int width, height;
		GetViewerWindowSize(&width, &height);
		aspectRatio = (float)width / (float)height;
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
		glViewport(0, 0, width, height);
	}

	///////////////////
	// WIREFRAME MODE
	if(_viewerRenderState.wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


	///////////////////
	// RENDER OBJECTS
	for(int i = 0; i < _viewerRenderState.renderList.Count(); i++) {
		RenderObject *renderObject = GetRenderObject(_viewerRenderState.renderList[i]);
		if(renderObject != NULL) {
			if(renderObject->hasTexture) {
				// render texture to quad here
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, renderObject->textureID);
			}

			glUseProgram(GetTextureShader());

			// TODO (rhoe) we dont need to update projection uniform every frame
			glm::mat4 projection = glm::perspective(glm::radians(45.0f),
													aspectRatio,
													0.1f, 1000.0f);
			GLuint projectionLoc = glGetUniformLocation(GetTextureShader(), "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

			glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
			model = glm::translate(model, glm::vec3(0, 0, 0));
			// model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0, 1, 0));
			GLuint modelLoc = glGetUniformLocation(GetTextureShader(), "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			GLuint VAO = GetCurrentContextVAO(renderObject->VAOHandle);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, renderObject->indicesCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		break;
	}

	///////////////////
	// RESET STATE
	_viewerRenderState.renderList.Clear();

	if(!ViewerInMain()) {
		glfwSwapBuffers(_viewerWindow);
		glfwMakeContextCurrent(_win);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
