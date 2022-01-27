#include "viewer_render.h"
#include "data.h"
#include "Camera.h"

void CreateViewerTextureRenderObject()
{
	_viewerRenderState.baseTextureObject = AddNewRenderObject();
	RenderObject *renderObject = GetRenderObjects()->Get(_viewerRenderState.baseTextureObject);

	// TODO (rhoe) currently we are wasting a gl id here
	//             perhaps we should pass vao as parameter instead of
	//             creating a new one
	renderObject->VAOHandle = CreateSpriteVAO();
	renderObject->hasTexture = true;
	renderObject->indicesCount = 6;
	renderObject->pos = vec3(0, 0, 0);
	renderObject->rot = vec3(0, 0, 0);
	renderObject->scale = vec3(1, 1, 1);
	renderObject->color = vec4(1, 1, 1, 1);

	Pixel white = Pixel(255, 255, 255, 255);
	glGenTextures(1, &_viewerRenderState.defaultTexture);
	glBindTexture(GL_TEXTURE_2D, _viewerRenderState.defaultTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &white);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

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
    _viewerRenderState.defaultShader = Shader("assets/texture.vert", "assets/texture.frag");
	_viewerRenderState.defaultShader.Use();
	CreateViewerTextureRenderObject();
}

// Takes the handle of texture resource
void AddTextureToRenderQueue(ObjectHandle handle)
{
	Texture *texture = GetTextures()->Get(handle);
	if(texture) {
		RenderObject *renderObject = GetRenderObjects()->Get(_viewerRenderState.baseTextureObject);
		glBindTexture(GL_TEXTURE_2D, renderObject->textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		RenderObjectInstance instance = RenderObjectInstance(_viewerRenderState.baseTextureObject);
		AddToRenderQueue(instance);
	}
}

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

void AddToRenderPointLightQueue(RenderLightInstance instance)
{
	_viewerRenderState.renderPointLights.Insert(instance);
}

void ViewerGLSettings()
{
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void DrawRenderObjectInstance(RenderObjectInstance *instance, glm::mat4 model)
{
	RenderObject *renderObject = GetRenderObjects()->Get(instance->renderObjectHandle);

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
	_viewerRenderState.defaultShader.Use();
			
	/////////////////
	// MODEL
	model = glm::translate(model, renderObject->pos);
	model = glm::translate(model, instance->pos);
	quat q = quat(renderObject->rot) * quat(instance->rot);
	glm::mat4 rotationMatrix = glm::mat4_cast(q);
	model = model * rotationMatrix;
	model = glm::scale(model, renderObject->scale * instance->scale);
	_viewerRenderState.defaultShader.SetUniform("model", model);

	glm::mat3 model3x3 = glm::mat3(model);
	glm::mat3 normalMatrix = glm::inverseTranspose(model3x3);
	_viewerRenderState.defaultShader.SetUniform("normalMatrix", normalMatrix);

	// set color
	_viewerRenderState.defaultShader.SetUniform("objectColor", instance->color);
	

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

	if((ViewerInMain() && PointInsideRect(mouse, viewerRect)) ||
	   (!ViewerInMain() && mouseInViewerWin))
		return true;

	return false;
}

mat4 GetProjectionViewerWindow()
{
	int width, height;
	GetViewerWindowSize(&width, &height);
	float aspectRatio = (float)width / (float)height;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f),
											aspectRatio,
											0.1f, 1000.0f);
	return projection;
}

mat4 GetProjectionMainWindowViewer()
{
	float aspectRatio = (float)VIEWER_SIZE / (float)VIEWER_SIZE;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f),
											aspectRatio,
											0.1f, 1000.0f);
	return projection;
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

		// mouse drag (orbit)
		dragOffset = mouse - viewer->lastDragPos;
		if(mouse_buttons[GLFW_MOUSE_BUTTON_3]) {
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
	viewer->defaultShader.Use();
	i32 lightCount = 0;
	for(i32 i = 0; i < viewer->renderPointLights.Count(); i++) {

		// TODO (rhoe) here we should check for maximum light count reached
		// PointLight *pointLight = GetPointLights()->Get(&viewer->renderPointLights[i].pointLightHandle);
		// if(pointLight) {
			
			char buffer[128];

			// get pointlight pos uniform
			sprintf(buffer, "pointLights[%d].pos", lightCount);
			viewer->defaultShader.SetUniform(buffer, viewer->renderPointLights[i].pos);

			// get pointlight color uniform
			sprintf(buffer, "pointLights[%d].color", lightCount);
			viewer->defaultShader.SetUniform(buffer, viewer->renderPointLights[i].color);

			lightCount++;
		// }
	}
	viewer->defaultShader.SetUniform("pointLightAmount", lightCount);

	/////////////////
	// PROJECTION
	// TODO (rhoe) we dont need to update projection uniform every frame
	glm::mat4 projection = glm::perspective(glm::radians(45.0f),
											aspectRatio,
											0.1f, 1000.0f);
	viewer->defaultShader.SetUniform("projection", projection);

	/////////////////
	// VIEW
	mat4 view = viewer->cam.GetViewMatrix();
	viewer->defaultShader.SetUniform("view", view);

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
		RenderObjectGroup *group = GetRenderObjectGroups()->Get(instance.renderGroupHandle);
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

	ImDraw3DRender();

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

vec3 GetViewerMouseRay()
{
	int mouseX, mouseY;
	int width, height;
	mat4 projection;
	mat4 view = _viewerRenderState.cam.GetViewMatrix();
	if(ViewerInMain()) {
		int mainWidth, mainHeight;
		GetWindowSize(&mainWidth, &mainHeight);
		mouseX = mouse.x - (mainWidth - VIEWER_SIZE);
		mouseY = mouse.y;
		width = VIEWER_SIZE;
		height = VIEWER_SIZE;
		projection = GetProjectionMainWindowViewer();
	}
	else {
		mouseX = mouse.x;
		mouseY = mouse.y;
		GetViewerWindowSize(&width, &height);
		projection = GetProjectionViewerWindow();
	}

	vec3 rayDir = RayFromMouseCoord(width, height, mouseX, mouseY, projection, view);

	return rayDir;
}

bool TransformArrowGizmo(vec3 pos, vec3 axis, vec3 color)
{
	bool result = false;
	float arrowSize = 0.2f;
	vec3 planeNormals[3] = {
		vec3(1, 0, 0),
		vec3(0, 0, 1),
		vec3(0, 1, 0),
	};

	vec3 rayDir = GetViewerMouseRay();
	vec3 seStart  =_viewerRenderState.cam.pos;
	vec3 seEnd = seStart + (rayDir * 1000.0f);
	float t;
	ImDraw3DSetColor(color);
	if(IntersectSegmentCylinder(seStart, seEnd, pos, pos + axis * arrowSize, 0.015f, &t)) {
		ImDraw3DSetColor(vec3(1, 1, 1));
		if(mouse_buttons[GLFW_MOUSE_BUTTON_1] && !_viewerRenderState.transformGizmo.isDragging) {
			for(i32 i = 0; i < ARRAY_SIZE(planeNormals); i++) {
				if(planeNormals[i] != axis) {
					vec3 q;
					if(IntersectSegmentPlane(seStart, seEnd, pos, planeNormals[i], &t, &q)) {
						_viewerRenderState.transformGizmo.dragStart = q;
						_viewerRenderState.transformGizmo.isDragging = true;
						_viewerRenderState.transformGizmo.dragAxisLock = axis;
						_viewerRenderState.transformGizmo.dragPlaneNormal = planeNormals[i];
						result = true;
						break;
					}
				}
			}
		}
	}
	ImDraw3DArrow(pos, pos + axis * arrowSize);

	return result;
}

void TransformPlaneGizmo(vec3 origin, vec3 normal, vec3 color)
{
	float gizmoSize = 0.07f;
	float gizmoPadding = 0.03f;

	vec3 forward = vec3(0, 0, 1);
	if(normal != WORLD_UP) {
		forward = glm::cross(WORLD_UP, normal);
	}
	vec3 right = glm::cross(normal, forward);

	vec3 gizmoPos = origin + (forward * (gizmoSize + gizmoPadding)) + (right * (gizmoSize + gizmoPadding));

	vec3 rayDir = GetViewerMouseRay();
	vec3 seStart  =_viewerRenderState.cam.pos;
	vec3 seEnd = seStart + (rayDir * 1000.0f);
	vec3 a = gizmoPos - (right * gizmoSize) - (forward * gizmoSize);
	vec3 b = gizmoPos - (right * gizmoSize) + (forward * gizmoSize);
	vec3 c = gizmoPos + (right * gizmoSize) + (forward * gizmoSize);
	vec3 d = gizmoPos + (right * gizmoSize) - (forward * gizmoSize);
	vec3 q;
	ImDraw3DSetColor(color);
	if(IntersectSegmentQuadDouble(seStart, seEnd, a, b, c, d, &q)) {
		ImDraw3DSetColor(vec3(1, 1, 1));
		if(mouse_buttons[GLFW_MOUSE_BUTTON_1] && !_viewerRenderState.transformGizmo.isDragging) {
			_viewerRenderState.transformGizmo.dragStart = q;
			_viewerRenderState.transformGizmo.isDragging = true;
			_viewerRenderState.transformGizmo.dragAxisLock = forward + right;
			_viewerRenderState.transformGizmo.dragPlaneNormal = normal;
		}
	}

	ImDraw3DPlane(gizmoPos, normal, gizmoSize);
}

void TransformGizmo(vec3 pos)
{
	TransformArrowGizmo(pos, vec3(1, 0, 0), vec3(1, 0, 0));
	TransformArrowGizmo(pos, vec3(0, 1, 0), vec3(0, 1, 0));
	TransformArrowGizmo(pos, vec3(0, 0, 1), vec3(0, 0, 1));

	TransformPlaneGizmo(pos, vec3(0, 1, 0), vec3(1, 0, 1));
	TransformPlaneGizmo(pos, vec3(-1, 0, 0), vec3(0, 1, 1));
	TransformPlaneGizmo(pos, vec3(0, 0, 1), vec3(1, 1, 0));

	// handle dragging
	vec3 rayDir = GetViewerMouseRay();
	vec3 seStart  =_viewerRenderState.cam.pos;
	vec3 seEnd = seStart + (rayDir * 1000.0f);
	if(_viewerRenderState.transformGizmo.isDragging) {
		if(!mouse_buttons[GLFW_MOUSE_BUTTON_1]) {
			_viewerRenderState.transformGizmo.isDragging = false;
		}
		else {
			vec3 planeNormal = _viewerRenderState.transformGizmo.dragPlaneNormal;
			vec3 axisLock = _viewerRenderState.transformGizmo.dragAxisLock;

			float t;
			vec3 q;
			if(IntersectSegmentPlane(seStart, seEnd, pos, planeNormal, &t, &q)) {
				SceneObject *selectedObject = GetSelectedSceneObject();
				if(selectedObject != NULL) {
					vec3 dragDir = q - _viewerRenderState.transformGizmo.dragStart;
					dragDir *= axisLock;
					selectedObject->pos += dragDir;
					_viewerRenderState.transformGizmo.dragStart = q;
				}	
			}
		}
	}

	ImDraw3DSetColor(vec3(1,1,1));
	ImDraw3DCube(pos, 0.01f);
}

void UpdateGizmos()
{
	if(_globalEditorState->viewerMode == VIEW_SCENE) {
		SceneObject *selectedObject = GetSelectedSceneObject();
		if(selectedObject != NULL) {
			TransformGizmo(selectedObject->pos);
		}
	}
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
		_viewerRenderState.cam.Reset();
	}
}
