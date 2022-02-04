#include "node_editor.h"
#include "data.h"

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

bool MouseInsideViewerRect()
{
	// int width, height;
	// GetWindowSize(&width, &height);
	// Rect viewerRect = {};
	// viewerRect.width = VIEWER_SIZE;
	// viewerRect.height = VIEWER_SIZE;
	// viewerRect.pos = vec2(width - VIEWER_SIZE, 0);

	// if((ViewerInMain() && PointInsideRect(mouse, viewerRect)) ||
	//    (!ViewerInMain() && mouseInViewerWin))
	// 	return true;

	// return false;

	return false;
}

void InitializeNodeEditor()
{

	_nodeEditorState = (NodeEditorState*)malloc(sizeof(NodeEditorState));

	NodeEditorState *editor = _nodeEditorState;
	// editor->draggedNode = {};
	editor->isDragging = false;
	// editor->hoverState.hoveringElement = false;

	// _nodeEditorState->viewerWidth = VIEWER_SIZE;
	// _nodeEditorState->viewerHeight = VIEWER_SIZE;
	editor->nodeMultiSelect = false;
	// editor->selectDragging = false;
	new (&editor->selectedNodes) VMArray<ObjectHandle>();

	editor->promptOpen = false;
	editor->promptSetFocus = false;
	new (&editor->promptCandidates) VMArray<String>();

	int promptCandidateFocus;

	// CREATE OUTPUT NODE
	// TODO (rhoe) move this to another place
	String name("output");
	AddNodeConstructor(name, OutputNodeOp, CreateOutputNode);

	i32 width, height;
	GetWindowSize(&width, &height);
	SetNextConstructPos(vec2((float)width / 2.0f, (float)height / 2.0f));

	NodeConstructor *constructor = GetNodeConstructors()->Get(name);
	editor->output = ConstructNode(name, constructor);

	editor->currentPage = 0;
	new(&editor->pages) VMArray<String>();
	editor->pages.Insert(AddString("main"));
}

Node *GetOutputNode()
{
	return GetNode(GetOutputHandle());
}

ObjectHandle GetOutputHandle()
{
	return _nodeEditorState->output;
}

u32 GetCurrentPage()
{
	return _nodeEditorState->currentPage;
}

void NodeGUI()
{
	NodeEditorState *editor = _nodeEditorState;

	/////////////////
	// PAGES
	/////////////////


	/////////////////
	// NODE LIST
	/////////////////
	ImGui::Begin("editor");

	ImGui::InputInt("page", &editor->currentPage);

	// ImGui::Spacing();
	// VMArray<String>* names = GetNodeNames();
	// for(i32 i = 0; i < names->Count(); i++) {
	// 	if(ImGui::Button((*names)[i].buffer)) {
	// 		NodeConstructor *nodeConstructor = GetNodeConstructors()->Get((*names)[i]);
	// 		ConstructNode((*names)[i], nodeConstructor);
	// 	}
	// }
	if(ImGui::Button("add page")) {
		editor->pages.Insert(AddString(""));
	}

	char buffer[128];
	for(i32 i = 0; i < editor->pages.Count(); i++) {
		sprintf(buffer, "%d:", i);
		if(ImGui::Button(buffer)) {
			editor->currentPage = i;
		}
		ImGui::SameLine();
		String *pageName = GetStrings()->Get(editor->pages[i]);
		if(pageName) {
			if(i != 0) {
				sprintf(buffer, "##pagename%d", i);
				if(ImGui::InputText(buffer, pageName->buffer, pageName->bufferSize)) {
					pageName->ReCalc();
				}
			}
			else {
				ImGui::Text(pageName->buffer);
			}
		}
	}


	ImGui::End();



	/////////////////
	// INPUT PROMPT
	/////////////////
	if(singleKeyPress(GLFW_KEY_N) && !imguiWantsKeyboard) {
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
			// editor->promptCandidates = NamesBeginningWith(editor->promptBuffer);
			NamesBeginningWith(&editor->promptCandidates, editor->promptBuffer);
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
				NodeConstructor *nodeConstructor = GetNodeConstructors()->Get(editor->promptCandidates[i]);
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

Rect GetNodeOutputRect(ObjectHandle handle)
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

Rect GetNodeInputRect(ObjectHandle handle, int inputIndex)
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

Rect GetNodeParamRect(ObjectHandle handle, int paramIndex)
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

void DrawNode(ObjectHandle handle)
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
		if(node->inputs[i].handle.isset && NodeExists(node->inputs[i].handle)) {
			Rect outputRect = GetNodeOutputRect(node->inputs[i].handle);
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

			if(node->params[i].nodeHandle.isset && NodeExists(node->params[i].nodeHandle)) {
				Rect outputRect = GetNodeOutputRect(node->params[i].nodeHandle);
				vec2 outputPos = GetRectCenter(outputRect);
				vec2 paramPos = GetRectCenter(paramRect);
				ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
				ImDrawLine(outputPos, paramPos);
			}
		}
	}
}

void DrawNodeOutline(ObjectHandle handle)
{
	float outlineMargin = 0.7f;
	NodeEditorState *editor = _nodeEditorState;

	Node *node = GetNode(handle);
	if(node && node->page == editor->currentPage) {
		Rect outline= {};
		outline.pos = node->rect.pos - vec2(outlineMargin, outlineMargin);
		outline.width = node->rect.width + outlineMargin;
		outline.height = node->rect.height + outlineMargin;

		ImDrawSetColor(vec4(1.0f, 1.0f, 0.0f, 0.7f));
		ImDrawRectOutline(outline, 1.5f);
	}
}

void UpdateHoverState()
{
	NodeEditorState *editor = _nodeEditorState;
	editor->hoverState.elementType = EDITOR_ELEMENT_NONE;

	bool foundElement = false;

	for(int i = 0; i < _nodeState->nodes.Count(); i++) {
		ObjectHandle handle = _nodeState->nodes.GetHandle(i);
		Node *node = _nodeState->nodes.Get(handle);
		if(node && node->page == editor->currentPage) {
			// CHECK INPUTS
			for(int j = 0; j < node->inputs.Count(); j++) {
				Rect inputRect = GetNodeInputRect(handle, j);
				if(PointInsideRect(mouse, inputRect)) {
					foundElement = true;
					editor->hoverState.nodeHandle = handle;
					editor->hoverState.elementType = EDITOR_ELEMENT_INPUT;
					editor->hoverState.ctxHandle = j;
					break;
				}
			}

			// CHECK PARAMS
			if(!foundElement) {
				i32 ctxIndex = 0;
				for(int j = 0; j < node->params.Count(); j++) {
					if(node->params[j].exposed) {
						Rect paramRect = GetNodeParamRect(handle, ctxIndex);
						if(PointInsideRect(mouse, paramRect)) {
							foundElement = true;
							editor->hoverState.nodeHandle = handle;
							editor->hoverState.elementType = EDITOR_ELEMENT_PARAM;
							editor->hoverState.ctxHandle = j;
						}
						ctxIndex++;
					}
				}
			}

			// CHECK OUTPUTS
			if(!foundElement) {
				Rect outputRect = GetNodeOutputRect(handle);
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

	editor->isDragging = true;
	editor->draggedType = editor->hoverState.elementType;
	// editor->nodeMultiSelect = false;

	// hovering element
	if(editor->hoverState.elementType != EDITOR_ELEMENT_NONE) {
		editor->draggedNode = editor->hoverState.nodeHandle;
		editor->draggedCtxHandle = editor->hoverState.ctxHandle;
		Node *node = GetNode(editor->hoverState.nodeHandle);
		editor->dragOffset = node->rect.pos - mouse;
	}
	else {
		// start box selecting
		editor->selectDragStart = mouse;
		editor->selectedNodes.Clear();
		// editor->nodeMultiSelect = true;
	}
}

void UpdateNodeContinueDragging()
{
	NodeEditorState *editor = _nodeEditorState;

	// HANDLE CONTINUE DRAGGING
	switch(editor->draggedType) {
		case EDITOR_ELEMENT_NODE: {
			Node *node = GetNode(editor->draggedNode);

			if(editor->nodeMultiSelect) {
				for(i32 i = 0; i < editor->selectedNodes.Count(); i++) {
					Node *multiNode = GetNode(editor->selectedNodes[i]);
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
			Rect inputRect = GetNodeInputRect(editor->draggedNode,
											  editor->draggedCtxHandle);
			vec2 inputPos = GetRectCenter(inputRect);
			ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
			ImDrawLine(mouse, inputPos);
			break;
		}
		case EDITOR_ELEMENT_PARAM: {
			Rect paramRect = GetNodeParamRect(editor->draggedNode,
											  editor->draggedCtxHandle);
			vec2 paramPos = GetRectCenter(paramRect);
			ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
			ImDrawLine(mouse, paramPos);
			break;
		}
		case EDITOR_ELEMENT_OUTPUT: {
			Rect outputRect = GetNodeOutputRect(editor->draggedNode);
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
				ObjectHandle outHandle = editor->hoverState.nodeHandle;
				ObjectHandle inHandle = editor->draggedNode;
				int ctxHandle = editor->draggedCtxHandle;
				ConnectNodeInput(inHandle, outHandle, ctxHandle);
			}
			else {
				Node *node = GetNode(editor->draggedNode);
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
				Node *node = GetNode(editor->draggedNode);
				if(node) {
					ObjectHandle outHandle = editor->hoverState.nodeHandle;
					ObjectHandle inHandle = editor->draggedNode;
					int ctxHandle = editor->draggedCtxHandle;
					node->changed = true;
					ConnectNodeParameter(inHandle, outHandle, ctxHandle);
				}
			}
			else {
				Node *node = GetNode(editor->draggedNode);
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
				ObjectHandle outHandle = editor->draggedNode;
				ObjectHandle inHandle = editor->hoverState.nodeHandle;
				int ctxHandle = editor->hoverState.ctxHandle;
				ConnectNodeInput(inHandle, outHandle, ctxHandle);
			}
			else if(editor->hoverState.elementType == EDITOR_ELEMENT_PARAM) {
				ObjectHandle outHandle = editor->draggedNode;
				ObjectHandle inHandle = editor->hoverState.nodeHandle;
				int ctxHandle = editor->hoverState.ctxHandle;
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
				Node *node = _nodeState->nodes.Get(handle);
				if(node && node->page == editor->currentPage) {
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
			// else if(editor->selectedNodes.Count() == 1) {
				// editor->viewerNode = editor->selectedNodes[0];
			// }
		}
	}
}

void UpdateNodeDragging()
{
	NodeEditorState *editor = _nodeEditorState;

	if(!MouseInsideViewerRect()) {
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

void UpdateNodeEditor()
{
	NodeEditorState *editor = _nodeEditorState;

	//////////////////
	// CHECK HOVERSTATE
	UpdateHoverState();
	if(!mouseInViewerWin)
		UpdateNodeDragging();


	SetInspectorObject(editor->draggedNode);

	if(keys[GLFW_KEY_SPACE]) {
		if(editor->draggedNode.isset && NodeExists(editor->draggedNode)) {
			// editor->viewerNode = editor->draggedNode;
			SetViewerNode(editor->draggedNode);
		}
	}
	if(keys[GLFW_KEY_DELETE]) {
		DeleteNode(editor->draggedNode);
	}

	//////////////////
	// DRAW NODES
	for(i32 i = 0; i < GetNodes()->Count(); i++) {
		ObjectHandle handle = _nodeState->nodes.GetHandle(i);
		Node *node = GetNodes()->Get(handle);
		if(node && node->page == editor->currentPage) {
			DrawNode(handle);
		}
	}

	// DRAW SELECTED NODE OUTLINES
	if(editor->nodeMultiSelect) {
		for(i32 i = 0; i < editor->selectedNodes.Count(); i++) {
			DrawNodeOutline(editor->selectedNodes[i]);
		}
	}
	if(editor->draggedNode.isset && NodeExists(editor->draggedNode)) {
		DrawNodeOutline(editor->draggedNode);
	}

	///////////////////
	// GUI
	NodeGUI();
}

void CleanupNodeEditor()
{
	free(_nodeEditorState);
}
