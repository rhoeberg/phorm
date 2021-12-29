#include "viewer_render.h"
#include "data.h"
#include "Camera.h"

void CreateViewerTextureRenderObject()
{
	// SETUP RENDEROBJECT FOR TEXTURE QUICK VIEW
	_viewerRenderState.baseTextureObject = AddNewRenderObject();
	RenderObject *renderObject = GetRenderObjects()->Get(&_viewerRenderState.baseTextureObject);

	// TODO (rhoe) currently we are wasting a gl id here
	//             perhaps we should pass vao as parameter instead of
	//             creating a new one
	renderObject->VAOHandle = CreateSpriteVAO();
	renderObject->hasTexture = true;
	renderObject->indicesCount = 12;

	Pixel white = Pixel(255, 255, 255, 255);
	glGenTextures(1, &_viewerRenderState.defaultTexture);
	glBindTexture(GL_TEXTURE_2D, _viewerRenderState.defaultTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &white);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void InitializeDefaultShader()
{
	// SETUP VIEW AND PROJECTION
    // glm::mat4 view = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
    // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));


    // glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);

	
	// CREATE DEFAULT SHADER
    _viewerRenderState.defaultShader = createShaderProgram("assets/texture.vert", "assets/texture.frag");
    glUseProgram(_viewerRenderState.defaultShader);
    // GLuint viewLoc = glGetUniformLocation(_viewerRenderState.defaultShader, "view");
    // GLuint projectionLoc = glGetUniformLocation(_viewerRenderState.defaultShader, "projection");
    // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    // glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void InitializeViewerFBO()
{
	// FRAMEBUFFER FOR MAIN WINDOW VIEWER
	glGenFramebuffers(1, &_viewerRenderState.fbo);
	glGenTextures(1, &_viewerRenderState.fboTexture);
	glBindTexture(GL_TEXTURE_2D, _viewerRenderState.fboTexture);
  
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _viewerRenderState.fboTexture, 0);  

	_viewerRenderState.quad = CreateSpriteVAO();

}

void InitializeViewerRender()
{
	InitializeDefaultShader();
	CreateViewerTextureRenderObject();
	// _viewerRenderState.orbitDistance = 5.0f;
	// _viewerRenderState.orbitDragSpeed = 0.006f;
}

// Takes the handle of texture resource
void AddTextureToRenderQueue(ObjectHandle *handle)
{
	Texture *texture = GetTextures()->Get(handle);
	if(texture) {
		RenderObject *renderObject = GetRenderObjects()->Get(&_viewerRenderState.baseTextureObject);
		glBindTexture(GL_TEXTURE_2D, renderObject->textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		RenderObjectInstance instance = RenderObjectInstance(_viewerRenderState.baseTextureObject);
		AddToRenderQueue(instance);
	}
}

// Takes the handle of RenderObject resource
void AddToRenderQueue(RenderObjectInstance instance)
{
	if(instance.renderObjectHandle.dataType == DATA_RENDEROBJECT) {
		_viewerRenderState.renderList.Insert(instance);
	}
}

void AddToRenderGroupQueue(RenderGroupInstance instance)
{
	if(instance.renderGroupHandle.dataType == DATA_RENDEROBJECT_GROUP) {
		_viewerRenderState.renderGroupList.Insert(instance);
	}
}

void AddToRenderPointLightQueue(ObjectHandle *handle)
{
	PointLight *light = GetPointLights()->Get(handle);
	if(light) {
		_viewerRenderState.renderPointLights.Insert(*handle);
	}
}

void ViewerGLSettings()
{
    // glEnable(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void DrawRenderObjectInstance(RenderObjectInstance *instance, glm::mat4 model)
{
	RenderObject *renderObject = GetRenderObjects()->Get(&instance->renderObjectHandle);

	///////////////
	// TEXTURE
	glActiveTexture(GL_TEXTURE0);
	if(renderObject->hasTexture) {
		// render texture to quad here
		glBindTexture(GL_TEXTURE_2D, renderObject->textureID);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, _viewerRenderState.defaultTexture);
	}

	/////////////////
	// SHADER
	glUseProgram(_viewerRenderState.defaultShader);
			
	/////////////////
	// MODEL
	model = glm::translate(model, renderObject->pos);
	model = glm::translate(model, instance->pos);
	quat q = quat(renderObject->rot) * quat(instance->rot);
	glm::mat4 rotationMatrix = glm::mat4_cast(q);
	model = model * rotationMatrix;
	model = glm::scale(model, renderObject->scale * instance->scale);
	GLuint modelLoc = glGetUniformLocation(_viewerRenderState.defaultShader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat3 model3x3 = glm::mat3(model);
	glm::mat3 normalMatrix = glm::inverseTranspose(model3x3);
	GLuint normalMatrixLoc = glGetUniformLocation(_viewerRenderState.defaultShader, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	/////////////////
	// DRAW
	GLuint VAO = GetCurrentContextVAO(renderObject->VAOHandle);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, renderObject->indicesCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

bool MouseInsideViewerRect()
{
	int width, height;
	GetWindowSize(&width, &height);
	Rect viewerRect = {};
	viewerRect.width = VIEWER_SIZE;
	viewerRect.height = VIEWER_SIZE;
	viewerRect.pos = vec2(width - VIEWER_SIZE, 0);

	if(ViewerInMain() && PointInsideRect(mouse, viewerRect) ||
	   !ViewerInMain() && mouseInViewerWin)
		return true;

	return false;
}

void UpdateViewerRender()
{
	ViewerRenderState *viewer = &_viewerRenderState;

	///////////////////
	// SETUP GL STATE
	ViewerGLSettings();

	///////////////////
	// HANDLE VIEWER INPUT
	if(MouseInsideViewerRect()) {
		if(scrollReady) {
			viewer->cam.OrbitScrollOut(scrollOffset);
			viewer->cam.OrbitDrag(vec2(0.0f, 0.0f));
			scrollReady = false;
		}

		vec2 dragOffset;

		// mouse drag
		dragOffset = mouse - viewer->lastDragPos;
		if(mouse_buttons[GLFW_MOUSE_BUTTON_1]) {
			viewer->cam.OrbitDrag(dragOffset);
		}
		else if(mouse_buttons[GLFW_MOUSE_BUTTON_2]) {
			viewer->cam.LookDir(dragOffset);
		}

		viewer->lastDragPos = mouse;

		if(keys[GLFW_KEY_W]) {
			viewer->cam.Move(CAM_FORWARD);
		}
		else if(keys[GLFW_KEY_S]) {
			viewer->cam.Move(CAM_BACKWARD);
		}

		if(keys[GLFW_KEY_A]) {
			viewer->cam.Move(CAM_LEFT);
		}
		else if(keys[GLFW_KEY_D]) {
			viewer->cam.Move(CAM_RIGHT);
		}
	}


	///////////////////
	// SETUP VIEWER MODE
	float aspectRatio;
	if(ViewerInMain()) {
		glfwMakeContextCurrent(_win);
	
		// glBindFramebuffer(GL_FRAMEBUFFER, viewer.fbo);
		// glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
		glViewport(0, 0, width, height);
	}

	///////////////////
	// WIREFRAME MODE
	if(viewer->wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	///////////////////
	// SETUP RENDER LIGHTS
	glUseProgram(viewer->defaultShader);
	i32 lightCount = 0;
	for(i32 i = 0; i < viewer->renderPointLights.Count(); i++) {

		// TODO (rhoe) here we should check for maximum light count reached
		PointLight *pointLight = GetPointLights()->Get(&viewer->renderPointLights[i]);
		if(pointLight) {
			
			char buffer[128];

			// get pointlight pos uniform
			sprintf(buffer, "pointLights[%d].pos", lightCount);
			GLuint posLoc = glGetUniformLocation(viewer->defaultShader, buffer);
			glUniform3fv(posLoc, 1, glm::value_ptr(pointLight->pos));

			// get pointlight color uniform
			sprintf(buffer, "pointLights[%d].color", lightCount);
			GLuint colorLoc = glGetUniformLocation(viewer->defaultShader, buffer);
			glUniform3fv(colorLoc, 1, glm::value_ptr(pointLight->color));

			lightCount++;
		}
	}
	GLuint lightAmountLoc = glGetUniformLocation(viewer->defaultShader, "pointLightAmount");
	glUniform1i(lightAmountLoc, lightCount);

	/////////////////
	// PROJECTION
	// TODO (rhoe) we dont need to update projection uniform every frame
	glm::mat4 projection = glm::perspective(glm::radians(45.0f),
											aspectRatio,
											0.1f, 1000.0f);
	GLuint projectionLoc = glGetUniformLocation(viewer->defaultShader, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	/////////////////
	// VIEW
	mat4 view = viewer->cam.GetViewMatrix();
	GLuint viewLoc = glGetUniformLocation(viewer->defaultShader, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	///////////////////
	// RENDER OBJECTS
	for(int i = 0; i < viewer->renderList.Count(); i++) {
		RenderObjectInstance instance = viewer->renderList[i];
		if(instance.renderObjectHandle.dataType == DATA_RENDEROBJECT) {
			glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
			DrawRenderObjectInstance(&instance, model);
		}
	}

	///////////////////
	// RENDER GROUPS
	for(int i = 0; i < viewer->renderGroupList.Count(); i++) {
		RenderGroupInstance instance = viewer->renderGroupList[i];
		RenderObjectGroup *group = GetRenderObjectGroups()->Get(&instance.renderGroupHandle);
		if(instance.renderGroupHandle.dataType == DATA_RENDEROBJECT_GROUP) {
			glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
			model = glm::translate(model, group->pos);
			quat q = quat(group->rot);
			model = model * toMat4(q);
			model = glm::scale(model, group->scale);

			for(i32 j = 0; j < group->renderObjects.Count(); j++) {
				DrawRenderObjectInstance(&group->renderObjects[j], model);
			}

		}
	}

	///////////////////
	// Draw gizmoz
	ImDraw3DSetView(view);
	ImDraw3DSetProjection(projection);
	UpdateGizmos();

	///////////////////
	// RESET STATE
	viewer->renderList.Clear();
	viewer->renderGroupList.Clear();
	viewer->renderPointLights.Clear();

	if(!ViewerInMain()) {
		glfwSwapBuffers(_viewerWindow);
		glfwMakeContextCurrent(_win);
	}
	else {
		// render viewer quad in main
		// glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, viewer.fboTexture);

		// glUseProgram(viewer.defaultShader);

		// glm::mat4 view = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
		// view = glm::translate(view, vec3(0, 0, 0));
		// GLuint viewLoc = glGetUniformLocation(viewer.defaultShader, "view");
		// glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		// glm::mat4 projection = glm::perspective(glm::radians(45.0f),
		// 										aspectRatio,
		// 										0.1f, 1000.0f);
		// GLuint projectionLoc = glGetUniformLocation(viewer.defaultShader, "projection");
		// glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
		// model = glm::translate(model, vec3(0, 0, 0));
		// // model = glm::scale(model, vec3(10, 10, 1));
		// GLuint modelLoc = glGetUniformLocation(viewer.defaultShader, "model");
		// glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// glBindVertexArray(viewer.quad);
		// glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void UpdateGizmos()
{
	ImDraw3DCube(vec3(0, 0, 0), 0.5f);
}

void UpdateViewerRenderGUI()
{
	ImGui::Begin("viewer");
	ImGui::Checkbox("wireframe", &_viewerRenderState.wireframe);
	if(ImGui::Button("toggle viewer window")) {
		ToggleViewer();
	}

	const char* viewerModes[] = { "object", "scene" };
	static int currentMode = 0;
	const char* previewValue = viewerModes[currentMode];
	if(ImGui::BeginCombo("mode", previewValue)) {

		for(i32 i = 0; i < ARRAY_SIZE(viewerModes); i++) {
			bool isSelected = (currentMode == i);
			if(ImGui::Selectable(viewerModes[i], isSelected)) {
				currentMode = i;
				SetViewerMode(currentMode);
			}

			if(isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	if(ImGui::Button("viewer mode")) {
		ToggleViewerMode();
	}
	ImGui::End();

	if(singleKeyPress(GLFW_KEY_R)) {
		// _viewerRenderState.orbitDistance = 5.0f;
		// _viewerRenderState.orbitDragSpeed = 0.006f;
		// _viewerRenderState.dragAmount = vec3(0, 0, 0);
		_viewerRenderState.cam.Reset();
		// _viewerRenderState.camPos = vec3(0, 0, -1
	}
}
