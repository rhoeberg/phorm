#define VIEWER_SIZE 600
global Shader defaultShader;
// global Camera cam;
global vec2 lastDragPos;

void InitializeSceneRender()
{
	_sceneRenderState = (SceneRenderState*)malloc(sizeof(SceneRenderState));
	new(&_sceneRenderState->commands) PArray<SceneRenderCommand>();

    defaultShader = Shader("assets/shaders/texture.vert", "assets/shaders/texture.frag");

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
	for(i32 i = 0; i < _sceneRenderState->commands.Count(); i++) {
		RenderSceneCommand(_sceneRenderState->commands[i]);
	}

	_sceneRenderState->commands.Clear();
}

void RenderSceneCommand(SceneRenderCommand command)
{
	// TODO (rhoe) we shouldnt have to do so much context switching
	// SetContextMain();
	// SetContextViewer();


	Scene *scene = GetScenes()->Get(command.scene);
	if(!scene) {
		ErrorLog("SCENE RENDER: could not find scene from handle");
		return;
	}

	SceneRenderData *renderData = GetSceneRenderDatas()->Get(command.sceneRenderData);
	if(!renderData) {
		ErrorLog("SCENE RENDER: could not find renderData from handle");
		return;
	}

	// i32 width, height;
	// GetViewerWindowSize(&width, &height);
	// texture->Create(width, height);
	// renderData->SetSize(width, height);
	GFXSetViewport(0, 0, renderData->width, renderData->height);

	// render scene to framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, renderData->fbo);

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

	GFXClear(scene->bgColor);

	// SHADER
	// testShader.Use();
	defaultShader.Begin();

	float aspectRatio = renderData->width / renderData->height;
	// float aspectRatio = (float)VIEWER_SIZE / (float)VIEWER_SIZE;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f),
											aspectRatio,
											0.1f, 1000.0f);

	mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

	mat4 view = {};
	Camera *sceneCam = GetCameras()->Get(scene->cameraHandle);
	if(FreeCamMode() || !sceneCam) {
		view = _viewerRenderState->freeCam.GetViewMatrix();
	}
	else {
		view = sceneCam->GetViewMatrix();
	}

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
		Node *node = GetNode(sceneObject->handle);
		if(node) {
			switch(node->type) {
				case DATA_RENDEROBJECT: {
					// TODO (rhoe) maybe we need to have more control over when GetData
					//             should call OP?
					ObjectHandle renderObjectHandle = node->GetData();
					RenderObjectInstance instance = sceneObject->ToRenderObjectInstance(renderObjectHandle);
					instance.Render(&defaultShader);
					break;
				}
			}
			// Node *node = GetNode(sceneObject->handle);
			// if(node) {
			// 	switch(node->type) {
			// 		case DATA_RENDEROBJECT: {
			// 			sceneObject->Render();
			// 			break;
			// 		}
			// 		case DATA_POINTLIGHT: {
			// 			break;
			// 		}
			// 	}

			// }
		}
	}
	defaultShader.End();
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// get texture from GPU
	if(command.texture.isset) {
		Bitmap *bitmap = GetBitmaps()->Get(command.texture);
		if(!bitmap) {
			ErrorLog("SceneDataToTexture: could not find texture from handle");
			return;
		}

		if(bitmap->width != renderData->width ||
		   bitmap->height != renderData->height) {
			ErrorLog("SceneDataToTexture: size mismatch between SceneData and Texture");
			return;
		}

		glBindBuffer(GL_PIXEL_PACK_BUFFER, renderData->pbo);
		glBindTexture(GL_TEXTURE_2D, renderData->textureID);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		void *mappedBuffer = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
		if(mappedBuffer) {
			memcpy(bitmap->pixels, mappedBuffer, sizeof(Pixel) * bitmap->width * bitmap->height);
			glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
		}
	}
}

void CleanupSceneRender()
{
	_sceneRenderState->commands.Free();
	free(_sceneRenderState);
}
