#define VIEWER_SIZE 600
global Shader defaultShader;
global Camera cam;
global vec2 lastDragPos;
global GLuint defaultTexture;

void InitializeSceneRender()
{
	_sceneRenderState = (SceneRenderState*)malloc(sizeof(SceneRenderState));
	new(&_sceneRenderState->commands) VMArray<SceneRenderCommand>();

    defaultShader = Shader("assets/shaders/texture.vert", "assets/shaders/texture.frag");

	// create default texture
	Pixel white = Pixel(255, 255, 255, 255);
	glGenTextures(1, &defaultTexture);
	glBindTexture(GL_TEXTURE_2D, defaultTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &white);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void AddSceneRenderCommand(ObjectHandle scene, ObjectHandle texture, ObjectHandle sceneRenderData)
{
	SceneRenderCommand command = {};
	command.scene = scene;
	command.texture = texture;
	command.sceneRenderData = sceneRenderData;
	_sceneRenderState->commands.Insert(command);
}

void UpdateSceneRender()
{
	///////////////////
	// UPDATE CAMERA
	if(MouseInsideViewerRect()) {
		if(scrollReady) {
			cam.OrbitScrollOut(scrollOffset);
			cam.OrbitDrag(vec2(0.0f, 0.0f));
			scrollReady = false;
		}

		vec2 dragOffset;

		// mouse drag (orbit)
		dragOffset = mouse - lastDragPos;
		if(mouse_buttons[GLFW_MOUSE_BUTTON_3]) {
			cam.OrbitDrag(dragOffset);
		}
		else if(mouse_buttons[GLFW_MOUSE_BUTTON_2]) {
			cam.LookDir(dragOffset);
		}

		lastDragPos = mouse;

		if(keys[GLFW_KEY_W]) {
			cam.Move(CAM_FORWARD);
		}
		else if(keys[GLFW_KEY_S]) {
			cam.Move(CAM_BACKWARD);
		}

		if(keys[GLFW_KEY_A]) {
			cam.Move(CAM_LEFT);
		}
		else if(keys[GLFW_KEY_D]) {
			cam.Move(CAM_RIGHT);
		}
	}


	for(i32 i = 0; i < _sceneRenderState->commands.Count(); i++) {
		RenderSceneCommand(_sceneRenderState->commands[i]);
	}

	_sceneRenderState->commands.Clear();
}

void RenderSceneCommand(SceneRenderCommand command)
{
	Scene *scene = GetScenes()->Get(command.scene);
	if(!scene) {
		ErrorLog("SCENE RENDER: could not find scene from handle");
		return;
	}

	Texture *texture = GetTextures()->Get(command.texture);
	if(!texture) {
		ErrorLog("SCENE RENDER: could not find texture from handle");
		return;
	}

	SceneRenderData *renderData = GetSceneRenderDatas()->Get(command.sceneRenderData);
	if(!renderData) {
		ErrorLog("SCENE RENDER: could not find renderData from handle");
		return;
	}


	glViewport(0, 0, texture->width, texture->height);

	// render scene to framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, renderData->fbo);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
	glClearColor(scene->bgColor.r, scene->bgColor.g, scene->bgColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

	// SHADER
	// testShader.Use();
	defaultShader.Use();

	// float aspectRatio = texture->width / texture->height;
	float aspectRatio = (float)VIEWER_SIZE / (float)VIEWER_SIZE;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f),
											aspectRatio,
											0.1f, 1000.0f);

	mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	mat4 view = cam.GetViewMatrix();

	defaultShader.SetUniform("projection", projection);
	defaultShader.SetUniform("view", view);

	// glBindVertexArray(testBox);
	// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	// // glDrawArrays(GL_TRIANGLES, 0, 3);
	// glBindVertexArray(0);

	// ADD SCENE POINTLIGHTS
	i32 lightCount = 0;
	char buffer[128];
	for(i32 i = 0; i < scene->pointLights.Count(); i++) {
		Node *node = GetNode(*scene->pointLights.GetAt(i));
		PointLight *light = GetPointLights()->Get(node->GetData());
		if(light) {
			// get pointlight pos uniform
			sprintf(buffer, "pointLights[%d].pos", lightCount);
			defaultShader.SetUniform(buffer, light->pos);

			// get pointlight color uniform
			sprintf(buffer, "pointLights[%d].color", lightCount);
			defaultShader.SetUniform(buffer, light->color);

			lightCount++;
		}
	}
	defaultShader.SetUniform("pointLightAmount", lightCount);


	// ////////////////
	// // RENDER ACTUAL 3D SCENE HERE
	for(i32 i = 0; i < scene->sceneObjects.Count(); i++) {
		SceneObject *sceneObject = scene->sceneObjects.GetAt(i);
		if(sceneObject) {
			Node *node = GetNode(sceneObject->handle);
			if(node) {
				switch(node->type) {
					case DATA_RENDEROBJECT: {
						RenderObject *renderObject = GetRenderObjects()->Get(node->GetData());

						// TODO (rhoe) we shouldnt have to do so much context switching
						SetContextMain();

						glActiveTexture(GL_TEXTURE0);
						if(renderObject->hasTexture) {
							glBindTexture(GL_TEXTURE_2D, renderObject->textureID);
						}
						else {
							glBindTexture(GL_TEXTURE_2D, defaultTexture);
						}

						// MODEL
						model = glm::translate(model, renderObject->pos);
						quat q = quat(renderObject->rot);
						mat4 rotationMatrix = glm::mat4_cast(q);
						model = model * rotationMatrix;
						model = glm::scale(model, renderObject->scale);
						// _viewerRenderState.defaultShader.SetUniform("model", model);
						defaultShader.SetUniform("model", model);

						mat3 model3x3 = mat3(model);
						mat3 normalMatrix = glm::inverseTranspose(model3x3);
						defaultShader.SetUniform("normalMatrix", normalMatrix);

						// set color
						defaultShader.SetUniform("objectColor", vec4(1, 1, 1, 1));
	
						// DRAW
						// TODO (rhoe) super dum hack, we need to be more clear about what context we should render to here, and get the correct VAO
						// also we dont want to do too much context switching

						// SetContextMain();
						GLuint VAO = GetMainContextVAO(renderObject->VAOHandle);
						// SetContextViewer();
						// GLuint VAO = GetCurrentContextVAO(renderObject->VAOHandle);
						glBindVertexArray(VAO);
						glDrawElements(GL_TRIANGLES, renderObject->indicesCount, GL_UNSIGNED_INT, 0);
						glBindVertexArray(0);

						break;
					}
					case DATA_POINTLIGHT: {
						break;
					}
				}

			}
		}
	}
	
	///////////////


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// get texture from GPU
	glBindBuffer(GL_PIXEL_PACK_BUFFER, renderData->pbo);
	glBindTexture(GL_TEXTURE_2D, renderData->textureID);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	void *mappedBuffer = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
	if(mappedBuffer) {
		memcpy(texture->pixels, mappedBuffer, sizeof(Pixel) * texture->width * texture->height);
		glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
	}
}
void CleanupSceneRender()
{
	_sceneRenderState->commands.Free();
	free(_sceneRenderState);
}
