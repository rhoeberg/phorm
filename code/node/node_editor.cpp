#include "node_editor.h"
#include "nodestate.h"

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
	_nodeEditorState->draggedNodeHandle = {};
	_nodeEditorState->hoverState.hoveringElement = false;

	_nodeEditorState->viewerWidth = VIEWER_SIZE;
	_nodeEditorState->viewerHeight = VIEWER_SIZE;

	_nodeEditorState->selectedNode = {};

	// create fbo + fbo texture
	// glGenTextures(1, &_nodeEditorState->fboTexture);
	// glBindTexture(GL_TEXTURE_2D, _nodeEditorState->fboTexture);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, VIEWER_SIZE, VIEWER_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glBindTexture(GL_TEXTURE_2D, 0);
	// glGenFramebuffers(1, &_nodeEditorState->fbo);
	// glBindFramebuffer(GL_FRAMEBUFFER, _nodeEditorState->fbo);
	// glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _nodeEditorState->fboTexture, 0);  
	// glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// create viewer shader
	// _nodeEditorState->viewerShader = createShaderProgram("assets/viewer.vert", "assets/viewer.frag");


	// int winWidth, winHeight;
	// GetWindowSize(&winWidth, &winHeight);
	// NodeEditorSetWindowSize(winWidth, winHeight);
}

void NodeGUI()
{
	ImGui::Begin("viewer");
	ImGui::Checkbox("wireframe", &_viewerRenderState.wireframe);
	if(ImGui::Button("toggle viewer window")) {
		ToggleViewer();
	}
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
	if(ImGui::Button("add video")) {
		AddVideoNode();
	}
	if(ImGui::Button("add renderobject")) {
		AddRenderObject();
	}
	ImGui::End();

	ImGui::Begin("Inspector");
	if(_nodeEditorState->draggedNodeHandle.type != NIL_NODE) {
		Node *node = GetNode(_nodeEditorState->draggedNodeHandle);

		ImGui::Text("%s", node->name);


		for(int i = 0; i < node->params.Count(); i++) {

			NodeParameter *param = &node->params[i];

			// EXPOSE
			ImGui::Checkbox("", &param->exposed);
			ImGui::SameLine();

			char buffer[128];
			sprintf(buffer, "%s", &param->name);

			switch(node->params[i].type) {
				case PARAM_INT: {
					if(ImGui::InputInt(buffer, &param->i)) {
						node->changed = true;
					}
					break;
				}
				case PARAM_DOUBLE: {
					if(ImGui::InputDouble(buffer, &node->params[i].d)) {
						node->changed = true;
					}
					// if(ImGui::SliderScalar(buffer, ImGuiDataType_Double, &param->d, &param->d_min, &param->d_max)) {
						// node->changed = true;
					// }
					break;
				}
				case PARAM_VEC3: {
					if(ImGui::InputFloat3(buffer, glm::value_ptr(param->v3))) {
						node->changed = true;
					}
					break;
				}
				case PARAM_STRING: {
					if(ImGui::InputText(buffer, param->str, ARRAY_SIZE(param->str))) {
						node->changed = true;
					}
					break;
				}
			}
		}
	}
	ImGui::End();
}

Rect GetNodeRect(NodeHandle handle)
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

Rect GetNodeOutputRect(NodeHandle handle)
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

Rect GetNodeInputRect(NodeHandle handle, int inputIndex)
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

Rect GetNodeParamRect(NodeHandle handle, int paramIndex)
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

void DrawNode(NodeHandle handle)
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
		if(node->inputs[i].handle.type != NIL_NODE) {
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
		Node *node = &_nodeState->nodes[i];
		if(node->type != NIL_NODE) {
			NodeHandle handle = {};
			handle.id = i;
			handle.type = node->type;

			// CHECK NODE
			Rect nodeRect = GetNodeRect(handle);
			if(PointInsideRect(mouse, nodeRect)) {
				_nodeEditorState->hoverState.nodeHandle = handle;
				_nodeEditorState->hoverState.hoveringElement = true;
				_nodeEditorState->hoverState.elementType = EDITOR_ELEMENT_NODE;
			}
			else {

				// CHECK INPUTS
				for(int j = 0; j < node->inputs.Count(); j++) {
					Rect inputRect = GetNodeInputRect(handle, j);
					if(PointInsideRect(mouse, inputRect)) {
						_nodeEditorState->hoverState.nodeHandle = handle;
						_nodeEditorState->hoverState.hoveringElement = true;
						_nodeEditorState->hoverState.elementType = EDITOR_ELEMENT_INPUT;
						_nodeEditorState->hoverState.ctxHandle = j;
						break;
					}
				}

				// CHECK INPUTS PARAM
				if(!_nodeEditorState->hoverState.hoveringElement) {
					for(int j = 0; j < node->params.Count(); j++) {
						Rect paramRect = GetNodeParamRect(handle, j);
						if(PointInsideRect(mouse, paramRect)) {
							_nodeEditorState->hoverState.nodeHandle = handle;
							_nodeEditorState->hoverState.hoveringElement = true;
							_nodeEditorState->hoverState.elementType = EDITOR_ELEMENT_PARAM;
							_nodeEditorState->hoverState.ctxHandle = j;
						}
					}
				}


				// CHECK OUTPUTS
				if(!_nodeEditorState->hoverState.hoveringElement) {
					Rect outputRect = GetNodeOutputRect(handle);
					if(PointInsideRect(mouse, outputRect)) {
						_nodeEditorState->hoverState.nodeHandle = handle;
						_nodeEditorState->hoverState.hoveringElement = true;
						_nodeEditorState->hoverState.elementType = EDITOR_ELEMENT_OUTPUT;
					}
				}
			}
		}
	}
}

void UpdateNodeDragging()
{
	// START DRAGGING
	if(_nodeEditorState->hoverState.hoveringElement) {
		if(!mouseInViewer && mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] && !_nodeEditorState->isDragging) {
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
						NodeHandle outHandle = _nodeEditorState->hoverState.nodeHandle;
						NodeHandle inHandle = _nodeEditorState->draggedNodeHandle;
						int ctxHandle = _nodeEditorState->draggedCtxHandle;
						ConnectNodes(inHandle, outHandle, ctxHandle);
					}
					else {
						Node *node = GetNode(_nodeEditorState->draggedNodeHandle);
						int ctxHandle = _nodeEditorState->draggedCtxHandle;
						node->changed = true;
						node->inputs[ctxHandle].handle.type = NIL_NODE;

					}
					break;
				}
				case EDITOR_ELEMENT_PARAM: {
					break;
				}
				case EDITOR_ELEMENT_OUTPUT: {
					if(_nodeEditorState->hoverState.elementType == EDITOR_ELEMENT_INPUT) {
						NodeHandle outHandle = _nodeEditorState->draggedNodeHandle;
						NodeHandle inHandle = _nodeEditorState->hoverState.nodeHandle;
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

void ShowNode(NodeHandle handle)
{
	Node *node = GetNode(handle);
	switch(node->type) {
		case TEXTURE_NODE: {
			AddTextureToRenderQueue(node->GetData());
			break;
		}
		case RENDEROBJECT_NODE: {
			AddToRenderQueue(node->GetData());
			break;
		}
	}
}

void UpdateNodeEditor()
{
	//////////////////
	// CHECK HOVERSTATE
	UpdateHoverState();

	UpdateNodeDragging();

	if(keys[GLFW_KEY_SPACE]) {
		if(_nodeEditorState->draggedNodeHandle.type != NIL_NODE) {
			_nodeEditorState->selectedNode = _nodeEditorState->draggedNodeHandle;
		}
	}

	//////////////////
	// DRAW NODES
	for(int i = 0; i < _nodeState->nodes.Count(); i++) {
		Node *node = &_nodeState->nodes[i];
		NodeHandle handle = {};
		handle.id = i;
		handle.type = node->type;
		DrawNode(handle);
	}

	//////////////////
	// VIEWER
	if(_nodeEditorState->selectedNode.type != NIL_NODE) {
		ShowNode(_nodeEditorState->selectedNode);
	}

	///////////////////
	// GUI
	NodeGUI();
}

void CleanupNodeEditor()
{
	free(_nodeEditorState);
}
