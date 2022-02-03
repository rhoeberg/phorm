/*
  How to view different things:
  the viewer could be a general purpose viewer that can view all kinds of objects.
  like:
  - the final output
  - renderobject
  - texture
  - scene
  - more?
  
  
  the viewer is simple it just renders a texture to a fullscreen quad.
  

  one way of having multiple different view sources could be to some way of settings which texture
  is being viewed.
  then if its the output we use the input of the output texture;
  if its from another source we need to generate a texture for the source.

  for this purpose we can have a general FBO and opengl texture object + the other needed buffers.
  when viewing a renderobject for example we then renders to these buffers and set the viewer texture
  to the general purpose texture object.

  to do this i guess we can have a textureID in the viewerstate which can be set and in the final viewer output we just render whatever this points to.

  
  to begin with we could use the "space" key funcionality and depending on the selected node
  type we can call a view function for that type. 
  how do we select output?

  
 */

int CreateViewerQuad()
{
    GLfloat vertices[] = {
		-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // top left
		 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, // top right
		 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, // bottom right
		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, // bottom left
    };

    GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
    };

    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	int VAOHandle = AddVAO();
	BindBuffersToVAO(VAOHandle, VBO, EBO);
	// BindBuffersToVAO(VAOHandle, VBO, EBO);

    return VAOHandle;
}

void InitializeViewerRender()
{
	_viewerRenderState = (ViewerRenderState*)malloc(sizeof(ViewerRenderState));
	_viewerRenderState->quadVAO = CreateViewerQuad();
	_viewerRenderState->shader = Shader("assets/shaders/viewer.vert", "assets/shaders/viewer.frag");
	glGenTextures(1, &_viewerRenderState->outputTextureID);
	glBindTexture(GL_TEXTURE_2D, _viewerRenderState->outputTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	_viewerRenderState->freeCam = Camera();

	// Initialize default viewer FBO and attached buffers
	_viewerRenderState->sceneRenderData.Initialize();
}
 
void ViewRenderObject()
{
	// render to general FBO
	// attach general texture (size of window?)
	// render texture on viewer quad

	Node *viewerNode = GetNode(GetViewerNode());
	ObjectHandle renderObjectHandle = viewerNode->GetData();
	RenderObject *renderObject = GetRenderObjects()->Get(renderObjectHandle);

	// glfwMakeContextCurrent(_viewerWindow);
	SetContextViewer();
	// SetContextMain();

	int width, height;
	GetViewerWindowSize(&width, &height);
	_viewerRenderState->sceneRenderData.SetSize(width, height);

	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, _viewerRenderState->sceneRenderData.fbo);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
	if(renderObject) {

		defaultShader.Use();

		glActiveTexture(GL_TEXTURE0);
		if(renderObject->hasTexture)
			glBindTexture(GL_TEXTURE_2D, renderObject->textureID);
		else
			glBindTexture(GL_TEXTURE_2D, defaultTexture);


		float aspectRatio = (float)width / (float)height;
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);

		mat4 view = _viewerRenderState->freeCam.GetViewMatrix();

		mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
		model = glm::translate(model, renderObject->pos);
		quat q = quat(renderObject->rot);
		glm::mat4 rotationMatrix = glm::mat4_cast(q);
		model = model * rotationMatrix;
		model = glm::scale(model, renderObject->scale);

		glm::mat3 model3x3 = glm::mat3(model);
		glm::mat3 normalMatrix = glm::inverseTranspose(model3x3);
		defaultShader.SetUniform("normalMatrix", normalMatrix);

		// set color
		defaultShader.SetUniform("objectColor", vec3(1, 0, 0));
		defaultShader.SetUniform("pointLightAmount", 0);

		defaultShader.SetUniform("projection", projection);
		defaultShader.SetUniform("view", view);
		defaultShader.SetUniform("model", model);

		/////////////////
		// DRAW
		GLuint VAO = GetCurrentContextVAO(renderObject->VAOHandle);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, renderObject->indicesCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// set viewer texture handle to fbo texture
	_viewerRenderState->currentViewTextureID = _viewerRenderState->sceneRenderData.textureID;
}

void UpdateViewerRender()
{
	////////////
	// UPDATE FREE CAM
	////////////
	// if(MouseInsideViewerRect()) {
	if(scrollReady) {
		_viewerRenderState->freeCam.OrbitScrollOut(scrollOffset);
		_viewerRenderState->freeCam.OrbitDrag(vec2(0.0f, 0.0f));
		scrollReady = false;
	}

	vec2 dragOffset = mouse - _viewerRenderState->lastDragPos;
	_viewerRenderState->lastDragPos = mouse;

	if(mouse_buttons[GLFW_MOUSE_BUTTON_3]) {
		_viewerRenderState->freeCam.OrbitDrag(dragOffset);
	}
	else if(mouse_buttons[GLFW_MOUSE_BUTTON_2]) {
		_viewerRenderState->freeCam.LookDir(dragOffset);
	}

	if(keys[GLFW_KEY_W]) {
		_viewerRenderState->freeCam.Move(CAM_FORWARD);
	}
	else if(keys[GLFW_KEY_S]) {
		_viewerRenderState->freeCam.Move(CAM_BACKWARD);
	}

	if(keys[GLFW_KEY_A]) {
		_viewerRenderState->freeCam.Move(CAM_LEFT);
	}
	else if(keys[GLFW_KEY_D]) {
		_viewerRenderState->freeCam.Move(CAM_RIGHT);
	}

	if(keys[GLFW_KEY_R]) {
		_viewerRenderState->freeCam.Reset();
	}

	glfwMakeContextCurrent(_viewerWindow);

	
	Node *viewerNode = GetNode(GetViewerNode());
	if(viewerNode != NULL && viewerNode->type == DATA_RENDEROBJECT) {
		ViewRenderObject();
	}
	// else {
	// 	glBindTexture(GL_TEXTURE_2D, _viewerRenderState->outputTextureID);
	// 	Pixel white = Pixel(255, 255, 255, 255);
	// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &white);
	// 	_viewerRenderState->currentViewTextureID = _viewerRenderState->outputTextureID;
	// }
	// RenderView((float)width / 2.0f, 0, (float)width / 2.0f, height);

	else {
	////////////
	// SET VIEWER TO OUTPUT TEXTURE
	////////////
		Node *output = GetOutputNode();
		Texture *texture = GetTextures()->Get(output->inputs[0]);
		glBindTexture(GL_TEXTURE_2D, _viewerRenderState->outputTextureID);
		if(texture) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
		}
		else {
			Pixel white = Pixel(255, 255, 255, 255);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &white);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		_viewerRenderState->currentViewTextureID = _viewerRenderState->outputTextureID;
	}

	int width, height;
	GetViewerWindowSize(&width, &height);
	RenderView(0, 0, width, height);

	glfwSwapBuffers(_viewerWindow);
	glfwMakeContextCurrent(_win);
}

void RenderView(i32 x, i32 y, i32 width, i32 height)
{
	glViewport(x, y, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

	_viewerRenderState->shader.Use();

	///////////////////
	// SETUP OUTPUT TEXTURE
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _viewerRenderState->currentViewTextureID);
	
	GLuint VAO = GetCurrentContextVAO(_viewerRenderState->quadVAO);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

void CleanupViewerRender()
{
	free(_viewerRenderState);
}
