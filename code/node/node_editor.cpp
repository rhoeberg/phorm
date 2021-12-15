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
	_nodeEditorState->draggedNode = {};
	_nodeEditorState->isDragging = false;
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
	ImGui::Begin("nodes");

	for(i32 i = 0; i < nodeNames.Count(); i++) {
		if(ImGui::Button(nodeNames[i].buffer)) {
			NodeConstructor *nodeConstructor = nodeConstructors.Get(nodeNames[i]);
			ConstructNode(nodeNames[i], nodeConstructor);
		}
	}

	// if(ImGui::Button("add blur")) {
	// 	AddBlurNode();
	// }
	// if(ImGui::Button("add mix")) {
	// 	AddMixTextureNode();
	// }
	// if(ImGui::Button("add load")) {
	// 	AddLoadTextureNode();
	// }
	// if(ImGui::Button("add cube")) {
	// 	AddCubeNode();
	// }
	// if(ImGui::Button("add grid")) {
	// 	AddGridNode();
	// }
	// if(ImGui::Button("add mesh noise")) {
	// 	AddMeshNoise();
	// }
	// if(ImGui::Button("add video")) {
	// 	AddVideoNode();
	// }
	// if(ImGui::Button("add renderobject")) {
	// 	AddRenderObject();
	// }
	// if(ImGui::Button("add time")) {
	// 	AddTimeNode();
	// }
	// if(ImGui::Button("add sin")) {
	// 	AddSinNode();
	// }
	// if(ImGui::Button("add label")) {
	// 	AddLabelNode();
	// }
	ImGui::End();
}

// Rect GetNodeRect(NodeHandle handle)
// {
// 	Node *node = GetNode(handle);

// 	Rect rect = {};
// 	rect.pos = node->pos;
// 	rect.width = NODE_BASE_WIDTH;
// 	rect.width += node->params.Count() * PARAM_WIDTH;
// 	rect.width += node->inputs.Count() * PARAM_WIDTH;
// 	rect.height = NODE_HEIGHT;

// 	return rect;
// }

Rect GetNodeOutputRect(ObjectHandle *handle)
{
	// Rect nodeRect = GetNodeRect(handle);
	Node *node = GetNode(handle);
	Rect nodeRect = node->rect;

	// DRAW OUTPUT
	Rect result = {};
	float outputOffsetX = (nodeRect.width / 2.0f) - PARAM_WIDTH / 2.0f;
	result.pos = nodeRect.pos + vec2(outputOffsetX, nodeRect.height);
	result.width = PARAM_WIDTH;
	result.height = PARAM_HEIGHT;

	return result;
}

Rect GetNodeInputRect(ObjectHandle *handle, int inputIndex)
{
	// Rect nodeRect = GetNodeRect(handle);
	Node *node = GetNode(handle);
	Rect nodeRect = node->rect;

	Rect inputRect = {};
	float offsetX = inputIndex * (PARAM_WIDTH + 10);
	float offsetY = -PARAM_HEIGHT;
	inputRect.pos = nodeRect.pos + vec2(offsetX, offsetY);
	inputRect.width = PARAM_WIDTH;
	inputRect.height = PARAM_HEIGHT;

	return inputRect;
}

Rect GetNodeParamRect(ObjectHandle *handle, int paramIndex)
{
	// Rect nodeRect = GetNodeRect(handle);
	Node *node = GetNode(handle);
	Rect nodeRect = node->rect;

	Rect paramRect = {};
	float offsetX = (paramIndex + node->inputs.Count()) * (PARAM_WIDTH + 10);
	float offsetY = -PARAM_HEIGHT;
	paramRect.pos = nodeRect.pos + vec2(offsetX, offsetY);
	paramRect.width = PARAM_WIDTH;
	paramRect.height = PARAM_HEIGHT;

	return paramRect;
}

void DrawNode(ObjectHandle *handle)
{
	Node *node = GetNode(handle);
	node->CallDraw();
	// CallNodeDrawFunction(node);

	if(node->type != DATA_NONE) {
		Rect outputRect = GetNodeOutputRect(handle);
		ImDrawSetColor(vec3(0.0f, 0.0f, 1.0f));
		ImDrawRect(outputRect);
	}

	// DRAW INPUTS
	for(int i = 0; i < node->inputs.Count(); i++) {

		Rect inputRect = GetNodeInputRect(handle, i);
		ImDrawSetColor(vec3(1.0f, 0.0f, 0.0f));
		ImDrawRect(inputRect);

		// TODO (rhoe) make it more clean / safe to see if a node input is set
		if(node->inputs[i].handle.isset && NodeExists(&node->inputs[i].handle)) {
			Rect outputRect = GetNodeOutputRect(&node->inputs[i].handle);
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

			if(node->params[i].nodeHandle.isset && NodeExists(&node->params[i].nodeHandle)) {
				Rect outputRect = GetNodeOutputRect(&node->params[i].nodeHandle);
				vec2 outputPos = GetRectCenter(outputRect);
				vec2 paramPos = GetRectCenter(paramRect);
				ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
				ImDrawLine(outputPos, paramPos);
			}
		}
	}
}

void UpdateHoverState()
{
	_nodeEditorState->hoverState.hoveringElement = false;
	for(int i = 0; i < _nodeState->nodes.Count(); i++) {
		ObjectHandle handle = _nodeState->nodes.GetHandle(i);
		Node *node = _nodeState->nodes.Get(&handle);
		if(node) {

			// CHECK NODE
			// Rect nodeRect = GetNodeRect(handle);
			Rect nodeRect = node->rect;
			if(PointInsideRect(mouse, nodeRect)) {
				_nodeEditorState->hoverState.nodeHandle = handle;
				_nodeEditorState->hoverState.hoveringElement = true;
				_nodeEditorState->hoverState.elementType = EDITOR_ELEMENT_NODE;
			}
			else {

				// CHECK INPUTS
				for(int j = 0; j < node->inputs.Count(); j++) {
					Rect inputRect = GetNodeInputRect(&handle, j);
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
						Rect paramRect = GetNodeParamRect(&handle, j);
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
					Rect outputRect = GetNodeOutputRect(&handle);
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
			_nodeEditorState->draggedNode = _nodeEditorState->hoverState.nodeHandle;
			_nodeEditorState->isDragging = true;
			_nodeEditorState->draggedType = _nodeEditorState->hoverState.elementType;
			_nodeEditorState->draggedCtxHandle = _nodeEditorState->hoverState.ctxHandle;

			Node *node = GetNode(&_nodeEditorState->hoverState.nodeHandle);
			_nodeEditorState->dragOffset = node->rect.pos - mouse;
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
						ObjectHandle *outHandle = &_nodeEditorState->hoverState.nodeHandle;
						ObjectHandle *inHandle = &_nodeEditorState->draggedNode;
						int ctxHandle = _nodeEditorState->draggedCtxHandle;
						ConnectNodeInput(inHandle, outHandle, ctxHandle);
					}
					else {
						Node *node = GetNode(&_nodeEditorState->draggedNode);
						if(node) {
							int ctxHandle = _nodeEditorState->draggedCtxHandle;
							node->changed = true;
							node->inputs[ctxHandle].handle.isset = false;
						}

					}
					break;
				}
				case EDITOR_ELEMENT_PARAM: {
					if(_nodeEditorState->hoverState.elementType == EDITOR_ELEMENT_OUTPUT) {
						Node *node = GetNode(&_nodeEditorState->draggedNode);
						if(node) {
							ObjectHandle *outHandle = &_nodeEditorState->hoverState.nodeHandle;
							ObjectHandle *inHandle = &_nodeEditorState->draggedNode;
							int ctxHandle = _nodeEditorState->draggedCtxHandle;
							node->changed = true;
							ConnectNodeParameter(inHandle, outHandle, ctxHandle);
						}
					}
					else {
						Node *node = GetNode(&_nodeEditorState->draggedNode);
						if(node) {
							int ctxHandle = _nodeEditorState->draggedCtxHandle;
							node->changed = true;
							node->params[ctxHandle].nodeHandle.isset = false;
						}
					}
					break;
				}
				case EDITOR_ELEMENT_OUTPUT: {
					if(_nodeEditorState->hoverState.elementType == EDITOR_ELEMENT_INPUT) {
						ObjectHandle *outHandle = &_nodeEditorState->draggedNode;
						ObjectHandle *inHandle = &_nodeEditorState->hoverState.nodeHandle;
						int ctxHandle = _nodeEditorState->hoverState.ctxHandle;
						ConnectNodeInput(inHandle, outHandle, ctxHandle);
					}
					else if(_nodeEditorState->hoverState.elementType == EDITOR_ELEMENT_PARAM) {
						ObjectHandle *outHandle = &_nodeEditorState->draggedNode;
						ObjectHandle *inHandle = &_nodeEditorState->hoverState.nodeHandle;
						int ctxHandle = _nodeEditorState->draggedCtxHandle;
						ConnectNodeParameter(inHandle, outHandle, ctxHandle);
					}
					break;
				}
			}
		}
		else {
			// HANDLE CONTINUE DRAGGING
			switch(_nodeEditorState->draggedType) {
				case EDITOR_ELEMENT_NODE: {
					Node *node = GetNode(&_nodeEditorState->draggedNode);
					node->rect.pos = mouse + _nodeEditorState->dragOffset;
					break;
				}
				case EDITOR_ELEMENT_INPUT: {
					Rect inputRect = GetNodeInputRect(&_nodeEditorState->draggedNode,
													  _nodeEditorState->draggedCtxHandle);
					vec2 inputPos = GetRectCenter(inputRect);
					ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
					ImDrawLine(mouse, inputPos);
					break;
				}
				case EDITOR_ELEMENT_PARAM: {
					Rect paramRect = GetNodeParamRect(&_nodeEditorState->draggedNode,
													  _nodeEditorState->draggedCtxHandle);
					vec2 paramPos = GetRectCenter(paramRect);
					ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
					ImDrawLine(mouse, paramPos);
					break;
				}
				case EDITOR_ELEMENT_OUTPUT: {
					Rect outputRect = GetNodeOutputRect(&_nodeEditorState->draggedNode);
					vec2 outputPos = GetRectCenter(outputRect);
					ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
					ImDrawLine(mouse, outputPos);
					break;
				}
			}
		}
	}
}

void ShowNode(ObjectHandle *handle)
{
	Node *node = GetNode(handle);
	switch(node->type) {
		case DATA_TEXTURE: {
			AddTextureToRenderQueue(&node->GetData());
			break;
		}
		case DATA_RENDEROBJECT: {
			AddToRenderQueue(&node->GetData());
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
		if(_nodeEditorState->draggedNode.isset && NodeExists(&_nodeEditorState->draggedNode)) {
			_nodeEditorState->selectedNode = _nodeEditorState->draggedNode;
		}
	}
	if(keys[GLFW_KEY_DELETE]) {
		DeleteNode(&_nodeEditorState->draggedNode);
	}

	//////////////////
	// DRAW NODES
	for(int i = 0; i < _nodeState->nodes.Count(); i++) {
		ObjectHandle handle = _nodeState->nodes.GetHandle(i);
		Node *node = _nodeState->nodes.Get(&handle);
		if(node) {
			DrawNode(&handle);
		}
	}

	//////////////////
	// SELECT FOR VIEWER
	if(_nodeEditorState->selectedNode.isset && NodeExists(&_nodeEditorState->selectedNode)) {
		ShowNode(&_nodeEditorState->selectedNode);
	}

	///////////////////
	// GUI
	NodeGUI();
}

void CleanupNodeEditor()
{
	free(_nodeEditorState);
}
