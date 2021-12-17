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

	NodeEditorState *editor = _nodeEditorState;
	editor->draggedNode = {};
	editor->isDragging = false;
	// editor->hoverState.hoveringElement = false;

	// _nodeEditorState->viewerWidth = VIEWER_SIZE;
	// _nodeEditorState->viewerHeight = VIEWER_SIZE;
	editor->nodeMultiSelect = false;
	// editor->selectDragging = false;
	new (&editor->selectedNodes) VMArray<ObjectHandle>();

	editor->selectedNode = {};

	editor->promptOpen = false;
	editor->promptSetFocus = false;
	new (&editor->promptCandidates) VMArray<String>();

	int promptCandidateFocus;

	// int winWidth, winHeight;
	// GetWindowSize(&winWidth, &winHeight);
	// NodeEditorSetWindowSize(winWidth, winHeight);
}

void NodeGUI()
{
	NodeEditorState *editor = _nodeEditorState;

	/////////////////
	// NODE LIST
	/////////////////
	ImGui::Begin("nodes");
	for(i32 i = 0; i < nodeNames.Count(); i++) {
		if(ImGui::Button(nodeNames[i].buffer)) {
			NodeConstructor *nodeConstructor = nodeConstructors.Get(nodeNames[i]);
			ConstructNode(nodeNames[i], nodeConstructor);
		}
	}
	ImGui::End();



	/////////////////
	// INPUT PROMPT
	/////////////////
	if(singleKeyPress(GLFW_KEY_N)) {
		if(!editor->promptOpen) {
			editor->promptOpen = true;
			editor->promptSetFocus = true;
			sprintf(editor->promptBuffer, "");
			ImGui::SetNextWindowPos(ImVec2(mouse.x, mouse.y));
			SetPromptActive(true);
			editor->promptCandidates.Clear();
			editor->promptCandidateSelected = 0;
		}
	}
	if(singleKeyPress(GLFW_KEY_ESCAPE)) {
		editor->promptOpen = false;
		SetPromptActive(false);
	}
	if(editor->promptOpen) {
		if(singleKeyPress(GLFW_KEY_DOWN)) {
			editor->promptCandidateSelected = (++editor->promptCandidateSelected % editor->promptCandidates.Count());
		}
		else if(singleKeyPress(GLFW_KEY_UP)) {
			editor->promptCandidateSelected = (--editor->promptCandidateSelected % editor->promptCandidates.Count());
			if(editor->promptCandidateSelected < 0)
				editor->promptCandidateSelected = editor->promptCandidates.Count() + editor->promptCandidateSelected;
		}

		i32 windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize;
		bool show = true;
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1, 1, 1, 1));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
		ImGui::Begin("add prompt", &show, windowFlags);
		if(editor->promptSetFocus) {
			ImGui::SetKeyboardFocusHere();
			editor->promptSetFocus = false;
		}
		if(ImGui::InputText("##newname", editor->promptBuffer, ARRAY_SIZE(editor->promptBuffer))) {
			editor->promptCandidates = NamesBeginningWith(editor->promptBuffer);
			editor->promptCandidateSelected = 0;
		}
		for(i32 i = 0; i < editor->promptCandidates.Count(); i++) {
			bool clickedOption = false;

			if(editor->promptCandidateSelected == i) {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7, 0.7, 0.7, 1));
				if(singleKeyPress(GLFW_KEY_ENTER)) {
					clickedOption = true;
				}
			}
			if(ImGui::Button(editor->promptCandidates[i].buffer)) {
				clickedOption = true;
			}
			if(editor->promptCandidateSelected == i) {
				ImGui::PopStyleColor();
			}

			if(clickedOption) {
				SetNextConstructPos(mouse);
				NodeConstructor *nodeConstructor = nodeConstructors.Get(editor->promptCandidates[i]);
				ConstructNode(editor->promptCandidates[i].buffer, nodeConstructor);
				editor->promptOpen = false;
				SetPromptActive(false);
			}
		}
		ImGui::End();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
	}
}

Rect GetNodeOutputRect(ObjectHandle *handle)
{
	Node *node = GetNode(handle);
	Rect nodeRect = node->rect;

	// DRAW OUTPUT
	Rect result = {};
	// float outputOffsetX = (nodeRect.width / 2.0f) - PARAM_WIDTH / 2.0f;
	result.pos = nodeRect.pos + vec2(0.0f, nodeRect.height - (PARAM_HEIGHT / 2.0f));
	result.width = PARAM_WIDTH;
	result.height = PARAM_HEIGHT;

	return result;
}

Rect GetNodeInputRect(ObjectHandle *handle, int inputIndex)
{
	Node *node = GetNode(handle);
	Rect nodeRect = node->rect;

	Rect inputRect = {};
	float offsetX = inputIndex * (PARAM_WIDTH + 10);
	// float offsetY = -PARAM_HEIGHT;
	inputRect.pos = nodeRect.pos + vec2(offsetX, -5.0f);
	inputRect.width = PARAM_WIDTH;
	inputRect.height = PARAM_HEIGHT;

	return inputRect;
}

Rect GetNodeParamRect(ObjectHandle *handle, int paramIndex)
{
	Node *node = GetNode(handle);
	Rect nodeRect = node->rect;

	Rect paramRect = {};
	float offsetX = (paramIndex + node->inputs.Count()) * (PARAM_WIDTH + 10);
	// float offsetY = -PARAM_HEIGHT;
	paramRect.pos = nodeRect.pos + vec2(offsetX, 0.0f - (PARAM_HEIGHT / 2.0f));
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
		ImDrawSetColor(vec3(0.6f, 0.0f, 0.2f));
		ImDrawRect(outputRect);
	}

	// DRAW INPUTS
	for(int i = 0; i < node->inputs.Count(); i++) {

		Rect inputRect = GetNodeInputRect(handle, i);
		ImDrawSetColor(vec3(0.0f, 0.5f, 0.2f));
		ImDrawRect(inputRect);

		// TODO (rhoe) make it more clean / safe to see if a node input is set
		if(node->inputs[i].handle.isset && NodeExists(&node->inputs[i].handle)) {
			Rect outputRect = GetNodeOutputRect(&node->inputs[i].handle);
			vec2 outputPos = GetRectCenter(outputRect);
			vec2 inputPos = GetRectCenter(inputRect);
			// ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
			ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
			ImDrawLine(outputPos, inputPos);
		}
	}

	// DRAW PARAMETERS
	int visibleParamsCount = 0;
	for(int i = 0; i < node->params.Count(); i++) {
		if(node->params[i].exposed) {
			Rect paramRect = GetNodeParamRect(handle, visibleParamsCount); 
			ImDrawSetColor(vec3(0.0f, 0.2f, 0.5f));
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
	NodeEditorState *editor = _nodeEditorState;
	editor->hoverState.elementType = EDITOR_ELEMENT_NONE;

	bool foundElement = false;

	for(int i = 0; i < _nodeState->nodes.Count(); i++) {
		ObjectHandle handle = _nodeState->nodes.GetHandle(i);
		Node *node = _nodeState->nodes.Get(&handle);
		if(node) {
			// CHECK INPUTS
			for(int j = 0; j < node->inputs.Count(); j++) {
				Rect inputRect = GetNodeInputRect(&handle, j);
				if(PointInsideRect(mouse, inputRect)) {
					foundElement = true;
					editor->hoverState.nodeHandle = handle;
					editor->hoverState.elementType = EDITOR_ELEMENT_INPUT;
					editor->hoverState.ctxHandle = j;
					break;
				}
			}

			// CHECK INPUTS PARAM
			if(!foundElement) {
				for(int j = 0; j < node->params.Count(); j++) {
					Rect paramRect = GetNodeParamRect(&handle, j);
					if(PointInsideRect(mouse, paramRect)) {
						foundElement = true;
						editor->hoverState.nodeHandle = handle;
						editor->hoverState.elementType = EDITOR_ELEMENT_PARAM;
						editor->hoverState.ctxHandle = j;
					}
				}
			}

			// CHECK OUTPUTS
			if(!foundElement) {
				Rect outputRect = GetNodeOutputRect(&handle);
				if(PointInsideRect(mouse, outputRect)) {
					foundElement = true;
					editor->hoverState.nodeHandle = handle;
					editor->hoverState.elementType = EDITOR_ELEMENT_OUTPUT;
				}
			}

			// CHECK NODE
			if(!foundElement) {
				Rect nodeRect = node->rect;
				if(PointInsideRect(mouse, nodeRect)) {
					foundElement = true;
					editor->hoverState.nodeHandle = handle;
					editor->hoverState.elementType = EDITOR_ELEMENT_NODE;
				}
			}
		}
	}
}

void UpdateNodeStartDragging()
{
	NodeEditorState *editor = _nodeEditorState;

	// start dragging something
	// if(mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] && !editor->isDragging) {
	editor->isDragging = true;
	editor->draggedType = editor->hoverState.elementType;

	// hovering element
	if(editor->hoverState.elementType != EDITOR_ELEMENT_NONE) {
		editor->draggedNode = editor->hoverState.nodeHandle;
		editor->draggedCtxHandle = editor->hoverState.ctxHandle;
		Node *node = GetNode(&editor->hoverState.nodeHandle);
		editor->dragOffset = node->rect.pos - mouse;
	}
	else {
		// start box selecting
		editor->selectDragStart = mouse;
		editor->nodeMultiSelect = false;
		editor->selectedNodes.Clear();
	}
	// }
}

void UpdateNodeContinueDragging()
{
	NodeEditorState *editor = _nodeEditorState;

	// HANDLE CONTINUE DRAGGING
	switch(editor->draggedType) {
		case EDITOR_ELEMENT_NODE: {
			Node *node = GetNode(&editor->draggedNode);

			if(editor->nodeMultiSelect) {
				for(i32 i = 0; i < editor->selectedNodes.Count(); i++) {
					Node *multiNode = GetNode(&editor->selectedNodes[i]);
					if(multiNode && multiNode != node) {
						vec2 offset = multiNode->rect.pos - node->rect.pos;
						multiNode->rect.pos = mouse + editor->dragOffset + offset;
					}
				}
			}

			node->rect.pos = mouse + editor->dragOffset;
			break;
		}
		case EDITOR_ELEMENT_INPUT: {
			Rect inputRect = GetNodeInputRect(&editor->draggedNode,
											  editor->draggedCtxHandle);
			vec2 inputPos = GetRectCenter(inputRect);
			ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
			ImDrawLine(mouse, inputPos);
			break;
		}
		case EDITOR_ELEMENT_PARAM: {
			Rect paramRect = GetNodeParamRect(&editor->draggedNode,
											  editor->draggedCtxHandle);
			vec2 paramPos = GetRectCenter(paramRect);
			ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
			ImDrawLine(mouse, paramPos);
			break;
		}
		case EDITOR_ELEMENT_OUTPUT: {
			Rect outputRect = GetNodeOutputRect(&editor->draggedNode);
			vec2 outputPos = GetRectCenter(outputRect);
			ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
			ImDrawLine(mouse, outputPos);
			break;
		}
		case EDITOR_ELEMENT_NONE: {
			// continue multi selecting
			vec2 a, b;
			a.x = Min(editor->selectDragStart.x, mouse.x);
			a.y = Min(editor->selectDragStart.y, mouse.y);
			b.x = Max(editor->selectDragStart.x, mouse.x);
			b.y = Max(editor->selectDragStart.y, mouse.y);

			Rect rect = Rect(a, b);
			ImDrawSetColor(vec4(0.0f, 0.0f, 1.0f, 0.3f));
			ImDrawSetNextLayer(1);
			ImDrawRect(rect);
			ImDrawSetNextLayer(0);
		}
	}
}

void UpdateNodeStopDragging()
{
	NodeEditorState *editor = _nodeEditorState;

	// handle stopped dragging
	editor->isDragging = false;
	switch(editor->draggedType) {
		case EDITOR_ELEMENT_NODE: {
			break;
		}
		case EDITOR_ELEMENT_INPUT: {
			if(editor->hoverState.elementType == EDITOR_ELEMENT_OUTPUT) {
				ObjectHandle *outHandle = &editor->hoverState.nodeHandle;
				ObjectHandle *inHandle = &editor->draggedNode;
				int ctxHandle = editor->draggedCtxHandle;
				ConnectNodeInput(inHandle, outHandle, ctxHandle);
			}
			else {
				Node *node = GetNode(&editor->draggedNode);
				if(node) {
					int ctxHandle = editor->draggedCtxHandle;
					node->changed = true;
					node->inputs[ctxHandle].handle.isset = false;
				}

			}
			break;
		}
		case EDITOR_ELEMENT_PARAM: {
			if(editor->hoverState.elementType == EDITOR_ELEMENT_OUTPUT) {
				Node *node = GetNode(&editor->draggedNode);
				if(node) {
					ObjectHandle *outHandle = &editor->hoverState.nodeHandle;
					ObjectHandle *inHandle = &editor->draggedNode;
					int ctxHandle = editor->draggedCtxHandle;
					node->changed = true;
					ConnectNodeParameter(inHandle, outHandle, ctxHandle);
				}
			}
			else {
				Node *node = GetNode(&editor->draggedNode);
				if(node) {
					int ctxHandle = editor->draggedCtxHandle;
					node->changed = true;
					node->params[ctxHandle].nodeHandle.isset = false;
				}
			}
			break;
		}
		case EDITOR_ELEMENT_OUTPUT: {
			if(editor->hoverState.elementType == EDITOR_ELEMENT_INPUT) {
				ObjectHandle *outHandle = &editor->draggedNode;
				ObjectHandle *inHandle = &editor->hoverState.nodeHandle;
				int ctxHandle = editor->hoverState.ctxHandle;
				ConnectNodeInput(inHandle, outHandle, ctxHandle);
			}
			else if(editor->hoverState.elementType == EDITOR_ELEMENT_PARAM) {
				ObjectHandle *outHandle = &editor->draggedNode;
				ObjectHandle *inHandle = &editor->hoverState.nodeHandle;
				int ctxHandle = editor->draggedCtxHandle;
				ConnectNodeParameter(inHandle, outHandle, ctxHandle);
			}
			break;
		}
		case EDITOR_ELEMENT_NONE: {
			// stop multi selecting

			vec2 a, b;
			a.x = Min(editor->selectDragStart.x, mouse.x);
			a.y = Min(editor->selectDragStart.y, mouse.y);
			b.x = Max(editor->selectDragStart.x, mouse.x);
			b.y = Max(editor->selectDragStart.y, mouse.y);

			for(i32 i = 0; i < _nodeState->nodes.Count(); i++) {
				ObjectHandle handle = _nodeState->nodes.GetHandle(i);
				Node *node = _nodeState->nodes.Get(&handle);
				if(node) {
					vec2 center = GetRectCenter(node->rect);
					if(center.x > a.x && center.x < b.x &&
					   center.y > a.y && center.y < b.y) {
						editor->selectedNodes.Insert(handle);
					}
				}
			}

			if(editor->selectedNodes.Count() > 1) {
				editor->nodeMultiSelect = true;
			}
			else if(editor->selectedNodes.Count() == 1) {
				editor->selectedNode = editor->selectedNodes[0];
			}
		}
	}
}

void UpdateNodeDragging()
{
	NodeEditorState *editor = _nodeEditorState;

	if(!mouseInViewer) {
		// start dragging something
		if(mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] && !editor->isDragging) {
			UpdateNodeStartDragging();
		}
		else if(editor->isDragging) {
			if(!mouse_buttons[GLFW_MOUSE_BUTTON_LEFT]) {
				UpdateNodeStopDragging();
			}
			else {
				UpdateNodeContinueDragging();
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
	if(!mouseInViewer)
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
	for(i32 i = 0; i < _nodeState->nodes.Count(); i++) {
		ObjectHandle handle = _nodeState->nodes.GetHandle(i);
		Node *node = _nodeState->nodes.Get(&handle);
		if(node) {
			DrawNode(&handle);
		}
	}

	// DRAW SELECTED NODE OUTLINES
	int outlineMargin = 0.7f;
	for(i32 i = 0; i < _nodeEditorState->selectedNodes.Count(); i++) {
		Node *node = GetNode(&_nodeEditorState->selectedNodes[i]);
		if(node) {
			Rect outline= {};
			outline.pos = node->rect.pos - vec2(outlineMargin, outlineMargin);
			outline.width = node->rect.width + outlineMargin;
			outline.height = node->rect.height + outlineMargin;

			ImDrawSetColor(vec4(1.0f, 1.0f, 0.0f, 0.7f));
			ImDrawRectOutline(outline, 1.5f);
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
