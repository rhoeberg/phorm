#include "node_editor.h"

#define VIEWER_SIZE 600
#define PARAM_WIDTH 40
#define PARAM_HEIGHT 20
#define NODE_HEIGHT 30
#define NODE_BASE_WIDTH 100

// TODO (rhoe) move these to seperate render system
struct ViewerRenderState {
	VMArray<int> renderList;
	int baseTextureObject;
	bool wireframe;
};

global ViewerRenderState _viewerRenderState;

void InitializeViewerRender()
{
    glfwMakeContextCurrent(_viewerWindow);
	_viewerRenderState.baseTextureObject = AddNewRenderObject();
	RenderObject *renderObject = GetRenderObject(_viewerRenderState.baseTextureObject);

	renderObject->VAO = createSpriteVAO();
	renderObject->hasTexture = true;
	renderObject->indicesCount = 12;
    glfwMakeContextCurrent(_win);
}


// void NodeEditorSetWindowSize(int width, int height)
// {
// 	//////////////
// 	// VIEWER IN MAIN LOCATION
// 	glUseProgram(_nodeEditorState->viewerShader);
//     GLuint projectionLoc = glGetUniformLocation(_nodeEditorState->viewerShader, "projection");
//     glm::mat4 projection;
//     projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
//     glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

//     GLuint modelLoc = glGetUniformLocation(_nodeEditorState->viewerShader, "model");
// 	glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
//     model = glm::translate(model, glm::vec3(width - (VIEWER_SIZE + 10), 10.0f, 0.0f));
//     model = glm::scale(model, glm::vec3(VIEWER_SIZE, VIEWER_SIZE,1));
//     glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
// }

void InitializeNodeEditor()
{
	_nodeEditorState = (NodeEditorState*)malloc(sizeof(NodeEditorState));
	_nodeEditorState->isDragging = false;
	_nodeEditorState->draggedNodeHandle = -1;
	_nodeEditorState->hoverState.hoveringElement = false;

	_nodeEditorState->viewerWidth = VIEWER_SIZE;
	_nodeEditorState->viewerHeight = VIEWER_SIZE;

	// create texture for node
	// glGenTextures(1, &_nodeEditorState->textureID);
	// glBindTexture(GL_TEXTURE_2D, _nodeEditorState->textureID);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glBindTexture(GL_TEXTURE_2D, 0);

	// create quad to render texture to
	// TODO (rhoe) make a cleaner API for creating objects
	// on specific windows/context. Perhaps keep duplicate VAO etc pr context?
    glfwMakeContextCurrent(_viewerWindow);
	_nodeEditorState->viewerQuad = createSpriteVAO();
    glfwMakeContextCurrent(_win);

	// create fbo + fbo texture

	glGenTextures(1, &_nodeEditorState->fboTexture);
	glBindTexture(GL_TEXTURE_2D, _nodeEditorState->fboTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, VIEWER_SIZE, VIEWER_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glGenFramebuffers(1, &_nodeEditorState->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _nodeEditorState->fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _nodeEditorState->fboTexture, 0);  
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// create viewer shader
	_nodeEditorState->viewerShader = createShaderProgram("assets/viewer.vert", "assets/viewer.frag");

	int winWidth, winHeight;
	GetWindowSize(&winWidth, &winHeight);
	// NodeEditorSetWindowSize(winWidth, winHeight);
}

void NodeGUI()
{
	ImGui::Begin("viewer");
	ImGui::Checkbox("wireframe", &_viewerRenderState.wireframe);
	ImGui::End();

	ImGui::Begin("nodes");
	if(ImGui::Button("add blur")) {
		AddBlurNode();
	}
	if(ImGui::Button("add mix")) {
		AddMixTextureNode();
	}
	if(ImGui::Button("add load")) {
		AddLoadTextureNode();
	}
	if(ImGui::Button("add cube")) {
		AddCubeNode();
	}
	if(ImGui::Button("add renderobject")) {
		AddRenderObject();
	}
	ImGui::End();

	ImGui::Begin("Inspector");
	if(_nodeEditorState->draggedNodeHandle != -1) {
		Node *node = GetNode(_nodeEditorState->draggedNodeHandle);

		ImGui::Text("%s", node->name);


		for(int i = 0; i < node->params.Count(); i++) {

			// EXPOSE
			ImGui::Checkbox("", &node->params[i].exposed);
			ImGui::SameLine();

			char buffer[128];
			sprintf(buffer, "%s", node->params[i].name);

			switch(node->params[i].type) {
				case PARAM_INT: {
					if(ImGui::InputInt(buffer, &node->params[i].i)) {
						node->changed = true;
					}
					break;
				}
				case PARAM_DOUBLE: {
					if(ImGui::InputDouble(buffer, &node->params[i].d)) {
						node->changed = true;
					}
					break;
				}
				case PARAM_VEC3: {
					if(ImGui::InputFloat3(buffer, glm::value_ptr(node->params[i].v3))) {
						node->changed = true;
					}
					break;
				}
				case PARAM_STRING: {
					if(ImGui::InputText(buffer, node->params[i].str, ARRAY_SIZE(node->params[i].str))) {
						node->changed = true;
					}
					break;
				}
			}
		}
	}
	ImGui::End();
}

Rect GetNodeRect(int handle)
{
	Node *node = GetNode(handle);

	Rect rect = {};
	rect.pos = node->pos;
	rect.width = NODE_BASE_WIDTH;
	rect.width += node->params.Count() * PARAM_WIDTH;
	rect.width += node->inputs.Count() * PARAM_WIDTH;
	rect.height = NODE_HEIGHT;

	return rect;
}

Rect GetNodeOutputRect(int handle)
{
	Rect nodeRect = GetNodeRect(handle);

	// DRAW OUTPUT
	Rect result = {};
	float outputOffsetX = (nodeRect.width / 2.0f) - PARAM_WIDTH / 2.0f;
	result.pos = nodeRect.pos + vec2(outputOffsetX, nodeRect.height);
	result.width = PARAM_WIDTH;
	result.height = PARAM_HEIGHT;

	return result;
}

Rect GetNodeInputRect(int handle, int inputIndex)
{
	Rect nodeRect = GetNodeRect(handle);

	Rect inputRect = {};
	float offsetX = inputIndex * (PARAM_WIDTH + 10);
	float offsetY = -PARAM_HEIGHT;
	inputRect.pos = nodeRect.pos + vec2(offsetX, offsetY);
	inputRect.width = PARAM_WIDTH;
	inputRect.height = PARAM_HEIGHT;

	return inputRect;
}

Rect GetNodeParamRect(int handle, int paramIndex)
{
	Node *node = GetNode(handle);
	Rect nodeRect = GetNodeRect(handle);

	Rect paramRect = {};
	float offsetX = (paramIndex + node->inputs.Count()) * (PARAM_WIDTH + 10);
	float offsetY = -PARAM_HEIGHT;
	paramRect.pos = nodeRect.pos + vec2(offsetX, offsetY);
	paramRect.width = PARAM_WIDTH;
	paramRect.height = PARAM_HEIGHT;

	return paramRect;
}

void DrawNode(int handle)
{
	Node *node = GetNode(handle);

	Rect rect = GetNodeRect(handle);
	ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
	ImDrawRect(rect);

	vec2 namePos = node->pos + vec2(10.0f, rect.height - 10.0f);
	ImDrawText(namePos, node->name);

	Rect outputRect = GetNodeOutputRect(handle);
	ImDrawSetColor(vec3(0.0f, 0.0f, 1.0f));
	ImDrawRect(outputRect);

	// DRAW INPUTS
	for(int i = 0; i < node->inputs.Count(); i++) {

		Rect inputRect = GetNodeInputRect(handle, i);
		ImDrawSetColor(vec3(1.0f, 0.0f, 0.0f));
		ImDrawRect(inputRect);

		// TODO (rhoe) make it more clean / safe to see if a node input is set
		if(node->inputs[i].handle != -1) {
			Rect outputRect = GetNodeOutputRect(node->inputs[i].handle);
			vec2 outputPos = GetRectCenter(outputRect);
			vec2 inputPos = GetRectCenter(inputRect);
			ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
			ImDrawLine(outputPos, inputPos);
		}
	}

	// DRAW PARAMETERS
	int visibleParamsCount = 0;
	for(int i = 0; i < node->params.Count(); i++) {
		if(node->params[i].exposed) {
			Rect paramRect = GetNodeParamRect(handle, visibleParamsCount); 
			ImDrawSetColor(vec3(0.0f, 1.0f, 0.0f));
			ImDrawRect(paramRect);
			visibleParamsCount++;
		}
	}
}

void UpdateHoverState()
{
	_nodeEditorState->hoverState.hoveringElement = false;
	for(int i = 0; i < _nodeState->nodes.Count(); i++) {
		Node *node = GetNode(i);

		// CHECK NODE
		Rect nodeRect = GetNodeRect(i);
		if(PointInsideRect(mouse, nodeRect)) {
			_nodeEditorState->hoverState.nodeHandle = i;
			_nodeEditorState->hoverState.hoveringElement = true;
			_nodeEditorState->hoverState.elementType = EDITOR_ELEMENT_NODE;
		}
		else {

			// CHECK INPUTS
			for(int j = 0; j < node->inputs.Count(); j++) {
				Rect inputRect = GetNodeInputRect(i, j);
				if(PointInsideRect(mouse, inputRect)) {
					_nodeEditorState->hoverState.nodeHandle = i;
					_nodeEditorState->hoverState.hoveringElement = true;
					_nodeEditorState->hoverState.elementType = EDITOR_ELEMENT_INPUT;
					_nodeEditorState->hoverState.ctxHandle = j;
					break;
				}
			}

			// CHECK INPUTS PARAM
			if(!_nodeEditorState->hoverState.hoveringElement) {
				for(int j = 0; j < node->params.Count(); j++) {
					Rect paramRect = GetNodeParamRect(i, j);
					if(PointInsideRect(mouse, paramRect)) {
						_nodeEditorState->hoverState.nodeHandle = i;
						_nodeEditorState->hoverState.hoveringElement = true;
						_nodeEditorState->hoverState.elementType = EDITOR_ELEMENT_PARAM;
						_nodeEditorState->hoverState.ctxHandle = j;
					}
				}
			}


			// CHECK OUTPUTS
			if(!_nodeEditorState->hoverState.hoveringElement) {
				Rect outputRect = GetNodeOutputRect(i);
				if(PointInsideRect(mouse, outputRect)) {
					_nodeEditorState->hoverState.nodeHandle = i;
					_nodeEditorState->hoverState.hoveringElement = true;
					_nodeEditorState->hoverState.elementType = EDITOR_ELEMENT_OUTPUT;
				}
			}
		}
	}
}

void UpdateNodeDragging()
{
	// START DRAGGING
	if(_nodeEditorState->hoverState.hoveringElement) {
		if(mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] && !_nodeEditorState->isDragging) {
			_nodeEditorState->draggedNodeHandle = _nodeEditorState->hoverState.nodeHandle;
			_nodeEditorState->isDragging = true;
			_nodeEditorState->draggedType = _nodeEditorState->hoverState.elementType;
			_nodeEditorState->draggedCtxHandle = _nodeEditorState->hoverState.ctxHandle;

			Node *node = GetNode(_nodeEditorState->hoverState.nodeHandle);
			_nodeEditorState->dragOffset = node->pos - mouse;
		}
	}

	if(_nodeEditorState->isDragging) {
		// HANDLE STOP DRAGGING
		if(!mouse_buttons[GLFW_MOUSE_BUTTON_LEFT]) {
			// handle stopped dragging
			_nodeEditorState->isDragging = false;


			switch(_nodeEditorState->draggedType) {
				case EDITOR_ELEMENT_NODE: {
					break;
				}
				case EDITOR_ELEMENT_INPUT: {
					if(_nodeEditorState->hoverState.elementType == EDITOR_ELEMENT_OUTPUT) {
						int outHandle = _nodeEditorState->hoverState.nodeHandle;
						int inHandle = _nodeEditorState->draggedNodeHandle;
						int ctxHandle = _nodeEditorState->draggedCtxHandle;
						ConnectNodes(inHandle, outHandle, ctxHandle);
					}
					else {
						Node *node = GetNode(_nodeEditorState->draggedNodeHandle);
						int ctxHandle = _nodeEditorState->draggedCtxHandle;
						node->changed = true;
						node->inputs[ctxHandle].handle = -1;

					}
					break;
				}
				case EDITOR_ELEMENT_PARAM: {
					break;
				}
				case EDITOR_ELEMENT_OUTPUT: {
					if(_nodeEditorState->hoverState.elementType == EDITOR_ELEMENT_INPUT) {
						int outHandle = _nodeEditorState->draggedNodeHandle;
						int inHandle = _nodeEditorState->hoverState.nodeHandle;
						int ctxHandle = _nodeEditorState->hoverState.ctxHandle;
						ConnectNodes(inHandle, outHandle, ctxHandle);
					}
					break;
				}
			}
		}
		else {
			// HANDLE CONTINUE DRAGGING
			switch(_nodeEditorState->draggedType) {
				case EDITOR_ELEMENT_NODE: {
					Node *node = GetNode(_nodeEditorState->draggedNodeHandle);
					node->pos = mouse + _nodeEditorState->dragOffset;
					break;
				}
				case EDITOR_ELEMENT_INPUT: {
					Rect inputRect = GetNodeInputRect(_nodeEditorState->draggedNodeHandle,
													  _nodeEditorState->draggedCtxHandle);
					vec2 inputPos = GetRectCenter(inputRect);
					ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
					ImDrawLine(mouse, inputPos);
					break;
				}
				case EDITOR_ELEMENT_PARAM: {
					Rect paramRect = GetNodeParamRect(_nodeEditorState->draggedNodeHandle,
													  _nodeEditorState->draggedCtxHandle);
					vec2 paramPos = GetRectCenter(paramRect);
					ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
					ImDrawLine(mouse, paramPos);
					break;
				}
				case EDITOR_ELEMENT_OUTPUT: {
					Rect outputRect = GetNodeOutputRect(_nodeEditorState->draggedNodeHandle);
					vec2 outputPos = GetRectCenter(outputRect);
					ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
					ImDrawLine(mouse, outputPos);
					break;
				}
			}

		}
	}
}

void ShowNode(int handle)
{
	Node *node = GetNode(_nodeEditorState->draggedNodeHandle);
	switch(node->type) {
		case TEXTURE_NODE: {
			RenderObject *renderObject = GetRenderObject(_viewerRenderState.baseTextureObject);

			Texture *texture = GetTexture(node->GetData());
			glBindTexture(GL_TEXTURE_2D, renderObject->textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
			_viewerRenderState.renderList.Insert(_viewerRenderState.baseTextureObject);
			break;
		}
		case RENDEROBJECT_NODE: {
			_viewerRenderState.renderList.Insert(node->GetData());
			break;
		}
	}
}

void UpdateViewerRender()
{
    glfwMakeContextCurrent(_viewerWindow);

	if(_viewerRenderState.wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


	int width, height;
	GetViewerWindowSize(&width, &height);
	float aspectRatio = (float)width / (float)height;
    glViewport(0, 0, width, height);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  


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
			glBindVertexArray(renderObject->VAO);
			glDrawElements(GL_TRIANGLES, renderObject->indicesCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		break;
	}

	_viewerRenderState.renderList.Clear();
	glfwSwapBuffers(_viewerWindow);
    glfwMakeContextCurrent(_win);
}

void UpdateNodeEditor()
{
	//////////////////
	// CHECK HOVERSTATE
	UpdateHoverState();

	UpdateNodeDragging();

	//////////////////
	// DRAW NODES
	for(int i = 0; i < _nodeState->nodes.Count(); i++) {
		DrawNode(i);
	}

	//////////////////
	// VIEWER
    glfwMakeContextCurrent(_viewerWindow);
	if(_nodeEditorState->draggedNodeHandle != -1) {
		ShowNode(_nodeEditorState->draggedNodeHandle);
	}
    glfwMakeContextCurrent(_win);


	///////////////////
	// GUI
	NodeGUI();
}

void CleanupNodeEditor()
{
	free(_nodeEditorState);
}
