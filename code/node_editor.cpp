#include "node_editor.h"
#include "data.h"

bool MouseInsideViewerRect()
{
	// int width, height;
	// GetWindowSize(&width, &height);
	// Rect viewerRect = {};
	// viewerRect.width = VIEWER_SIZE;
	// viewerRect.height = VIEWER_SIZE;
	// viewerRect.pos = vec2(width - VIEWER_SIZE, 0);

	// if((ViewerInMain() && NodeEditorPointInsideRect(mouse, viewerRect)) ||
	//    (!ViewerInMain() && mouseInViewerWin))
	// 	return true;

	// return false;

	return false;
}

void InitializeNodeEditor()
{

	_nodeEditorState = (NodeEditorState*)malloc(sizeof(NodeEditorState));

	NodeEditorState *editor = _nodeEditorState;

	editor->editorOffset = vec2(0, 0);
	editor->zoom = 1.0f;

	editor->isDragging = false;
	editor->nodeMultiSelect = false;
	new (&editor->selectedNodes) PArray<ObjectHandle>();

	editor->promptOpen = false;
	editor->promptSetFocus = false;
	new (&editor->promptCandidates) PArray<String>();

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
}

Node *GetOutputNode()
{
	return GetNode(GetOutputHandle());
}

ObjectHandle GetOutputHandle()
{
	return _nodeEditorState->output;
}

void PasteNodes()
{
	NodeEditorState *editor = _nodeEditorState;

	PMap<ObjectHandle, ObjectHandle> handleTable;
	PArray<ObjectHandle> newNodes;

	for(i32 i = 0; i < editor->selectedNodes.Count(); i++) {
		Node *node = GetNode(editor->selectedNodes[i]);
		ObjectHandle newHandle = node->Copy();
		handleTable.Insert(editor->selectedNodes[i], newHandle);
		newNodes.Insert(newHandle);
	}

	editor->selectedNodes.Clear();
	for(i32 i = 0; i < newNodes.Count(); i++) {
		Node *node = GetNode(newNodes[i]);

		for(i32 in = 0; in < node->inputs.Count(); in++) {
			if(handleTable.Exist(node->inputs[in].handle)) {
				node->inputs[in].handle = *handleTable.Get(node->inputs[in].handle);
			}
			else {
				node->inputs[in].handle.isset = false;
			}
		}

		for(i32 par = 0; par < node->params.Count(); par++) {
			if(handleTable.Exist(node->params[par].nodeHandle)) {
				node->params[par].nodeHandle = *handleTable.Get(node->params[par].nodeHandle);
			}
			else {
				node->params[par].nodeHandle.isset = false;
			}
		}


		editor->selectedNodes.Insert(newNodes[i]);
	}
}

// TODO (rhoe) move this to global editor
void NodeGUI()
{
	NodeEditorState *editor = _nodeEditorState;

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
				f32 zoomOffset = 1.0f / editor->zoom;
				SetNextConstructPos((mouse + editor->editorOffset) * zoomOffset);
				// SetNextConstructPos(mouse);
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
	// result.pos = nodeRect.pos + vec2(0.0f, nodeRect.height - (PARAM_HEIGHT / 2.0f));
	result.pos = nodeRect.pos + vec2(nodeRect.width - (PARAM_WIDTH / 2.0f), 15.0f);
	result.width = PARAM_WIDTH;
	result.height = PARAM_HEIGHT;

	return result;
}

Rect GetNodeInputRect(ObjectHandle handle, int index)
{
	Node *node = GetNode(handle);
	Rect nodeRect = node->rect;

	Rect rect = {};
	float offsetY = (index * (PARAM_WIDTH + PARAM_SPACING)) + NODE_HEADER_HEIGHT;
	// float offsetY = -PARAM_HEIGHT;
	rect.pos = nodeRect.pos + vec2(-(PARAM_WIDTH / 2.0f), offsetY);
	rect.width = PARAM_WIDTH;
	rect.height = PARAM_HEIGHT;

	return rect;
}

// Rect GetNodeParamRect(ObjectHandle handle, int paramIndex)
// {
// 	Node *node = GetNode(handle);
// 	Rect nodeRect = node->rect;

// 	Rect paramRect = {};
// 	float offsetX = (paramIndex + node->inputs.Count()) * (PARAM_WIDTH + 10);
// 	paramRect.pos = nodeRect.pos + vec2(offsetX, 0.0f - (PARAM_HEIGHT / 2.0f));
// 	paramRect.width = PARAM_WIDTH;
// 	paramRect.height = PARAM_HEIGHT;

// 	return paramRect;
// }

void DrawNodeEditorRect(Rect rect, i32 layer, vec3 color)
{
	ImDrawSetColor(color);
	ImDrawSetNextLayer(layer);
	Rect offsetRect = rect;
	offsetRect.pos += _nodeEditorState->editorOffset;
	offsetRect.pos *= _nodeEditorState->zoom;
	offsetRect.width *= _nodeEditorState->zoom;
	offsetRect.height *= _nodeEditorState->zoom;
	ImDrawRect(offsetRect);
}

void DrawNodeEditorLine(vec2 start, vec2 end, i32 layer, vec3 color, f32 thickness)
{
	ImDrawSetColor(color);
	ImDrawSetNextLayer(layer);
	vec2 offsetStart = start + _nodeEditorState->editorOffset;
	offsetStart *= _nodeEditorState->zoom;
	vec2 offsetEnd = end + _nodeEditorState->editorOffset;
	offsetEnd *= _nodeEditorState->zoom;
	ImDrawLine(offsetStart, offsetEnd, thickness * _nodeEditorState->zoom);
}

void DrawNodeEditorText(vec2 pos, char *buffer, vec3 color)
{
	vec2 offsetPos = pos + _nodeEditorState->editorOffset;
	offsetPos *= _nodeEditorState->zoom;
	ImDrawText(offsetPos, buffer, color);
}

vec2 GetNodeEditorRectCenter(Rect rect)
{
	Rect offsetRect = rect;
	offsetRect.pos += _nodeEditorState->editorOffset;
	// offsetRect.pos *= _nodeEditorState->zoom;
	// offsetRect.width *= _nodeEditorState->zoom;
	// offsetRect.height *= _nodeEditorState->zoom;
	return GetRectCenter(offsetRect);
}

bool NodeEditorPointInsideRect(vec2 point, Rect rect)
{
	Rect offsetRect = rect;
	offsetRect.pos += _nodeEditorState->editorOffset;
	offsetRect.pos *= _nodeEditorState->zoom;
	offsetRect.width *= _nodeEditorState->zoom;
	offsetRect.height *= _nodeEditorState->zoom;
	return PointInsideRect(point, offsetRect);
}

void BaseNodeDrawFunc(Node *node)
{
	node->rect.height = NODE_BASE_WIDTH;
	node->rect.height += node->params.Count() * PARAM_WIDTH + PARAM_SPACING;
	node->rect.height += node->inputs.Count() * PARAM_WIDTH + PARAM_SPACING;
	node->rect.width = NODE_HEIGHT;

	// node->rect.height *= _nodeEditorState->zoom;
	// node->rect.width *= _nodeEditorState->zoom;
	DrawNodeEditorRect(node->rect, 1, COLOR_NODE_FILL);

	// vec2 namePos = node->rect.pos + vec2(8.0f, node->rect.height - 8.0f);
	vec2 namePos = node->rect.pos + vec2(2.0f, 10.0f);
	DrawNodeEditorText(namePos, node->name, COLOR_NODE_TEXT);
}

void DrawNode(ObjectHandle handle)
{
	// ImDrawSetNextLayer(1);

	Node *node = GetNode(handle);
	node->CallDraw();
	// CallNodeDrawFunction(node);


	if(node->type != DATA_NONE) {
		Rect outputRect = GetNodeOutputRect(handle);
		DrawNodeEditorRect(outputRect, 1, COLOR_NODE_OUTPUT);
	}

	// DRAW INPUTS
	for(int i = 0; i < node->inputs.Count(); i++) {
		Rect inputRect = GetNodeInputRect(handle, i);
		DrawNodeEditorRect(inputRect, 1, COLOR_NODE_INPUT);
		if(node->inputs[i].handle.isset && NodeExists(node->inputs[i].handle)) {
			Rect outputRect = GetNodeOutputRect(node->inputs[i].handle);
			vec2 outputPos = GetRectCenter(outputRect);
			vec2 inputPos = GetRectCenter(inputRect);
			DrawNodeEditorLine(outputPos, inputPos, 0, vec3(1.0f, 1.0f, 1.0f), 2.0f);
		}
	}

	// DRAW PARAMETERS
	int visibleParamsCount = 0;
	for(int i = 0; i < node->params.Count(); i++) {
		if(node->params[i].exposed) {
			i32 index = visibleParamsCount + node->inputs.Count();
			// Rect paramRect = GetNodeParamRect(handle, visibleParamsCount); 
			Rect paramRect = GetNodeInputRect(handle, index); 
			DrawNodeEditorRect(paramRect, 1, COLOR_NODE_INPUT);
			visibleParamsCount++;

			if(node->params[i].nodeHandle.isset && NodeExists(node->params[i].nodeHandle)) {
				Rect outputRect = GetNodeOutputRect(node->params[i].nodeHandle);
				vec2 outputPos = GetRectCenter(outputRect);
				vec2 paramPos = GetRectCenter(paramRect);
				DrawNodeEditorLine(outputPos, paramPos, 0, vec3(1.0f, 1.0f, 1.0f), 2.0f);
			}
		}
	}
}

void DrawNodeOutline(ObjectHandle handle)
{
	float outlineMargin = 0.7f;
	NodeEditorState *editor = _nodeEditorState;

	Node *node = GetNode(handle);
	if(node && node->page == GetCurrentPage()) {
		Rect outline= {};
		outline.pos = node->rect.pos - vec2(outlineMargin, outlineMargin);
		outline.pos += editor->editorOffset;
		outline.width = node->rect.width + outlineMargin;
		outline.height = node->rect.height + outlineMargin;

		outline.pos *= editor->zoom;
		outline.width *= editor->zoom;
		outline.height *= editor->zoom;

		ImDrawSetColor(COLOR_NODE_HIGHLIGHT);
		ImDrawRectOutline(outline, 2.5f);
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
		if(node && node->page == GetCurrentPage()) {
			// CHECK INPUTS
			for(int j = 0; j < node->inputs.Count(); j++) {
				Rect inputRect = GetNodeInputRect(handle, j);
				if(NodeEditorPointInsideRect(mouse, inputRect)) {
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
						i32 index = ctxIndex  + node->inputs.Count();
						// Rect paramRect = GetNodeParamRect(handle, ctxIndex);
						Rect paramRect = GetNodeInputRect(handle, ctxIndex);
						if(NodeEditorPointInsideRect(mouse, paramRect)) {
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
				if(NodeEditorPointInsideRect(mouse, outputRect)) {
					foundElement = true;
					editor->hoverState.nodeHandle = handle;
					editor->hoverState.elementType = EDITOR_ELEMENT_OUTPUT;
				}
			}

			// CHECK NODE
			if(!foundElement) {
				Rect nodeRect = node->rect;
				if(NodeEditorPointInsideRect(mouse, nodeRect)) {
					foundElement = true;
					editor->hoverState.nodeHandle = handle;
					editor->hoverState.elementType = EDITOR_ELEMENT_NODE;
				}
			}
		}
	}
}

bool HoveringNode()
{
	if(_nodeEditorState->hoverState.elementType == EDITOR_ELEMENT_NODE) {
		return true;
	}

	return false;
}

ObjectHandle GetHoverNodeHandle()
{
	return _nodeEditorState->hoverState.nodeHandle;
}

void UpdateNodeStartDragging()
{
	NodeEditorState *editor = _nodeEditorState;

	if(!EditorIsFrozen()) {
		editor->isDragging = true;
		editor->draggedType = editor->hoverState.elementType;

		// hovering element
		if(editor->hoverState.elementType != EDITOR_ELEMENT_NONE) {
			editor->draggedNode = editor->hoverState.nodeHandle;
			editor->draggedCtxHandle = editor->hoverState.ctxHandle;
			Node *node = GetNode(editor->hoverState.nodeHandle);
			float zoomOffset = 1.0f / editor->zoom;
			editor->dragOffset = node->rect.pos - (mouse * zoomOffset);
		}
		else {
			// start box selecting
			editor->selectDragStart = mouse;
			editor->selectedNodes.Clear();
		}
	}
}

void UpdateNodeContinueDragging()
{
	NodeEditorState *editor = _nodeEditorState;

	// HANDLE CONTINUE DRAGGING
	switch(editor->draggedType) {
		case EDITOR_ELEMENT_NODE: {
			float zoomOffset = 1.0f / editor->zoom;
			Node *node = GetNode(editor->draggedNode);

			if(editor->nodeMultiSelect) {
				for(i32 i = 0; i < editor->selectedNodes.Count(); i++) {
					Node *multiNode = GetNode(editor->selectedNodes[i]);
					if(multiNode && multiNode != node) {
						vec2 offset = multiNode->rect.pos - node->rect.pos;
						multiNode->rect.pos = (mouse * zoomOffset) + editor->dragOffset + offset;
					}
				}
			}

			node->rect.pos = (mouse * zoomOffset) + editor->dragOffset;
			break;
		}
		case EDITOR_ELEMENT_INPUT: {
			Rect inputRect = GetNodeInputRect(editor->draggedNode,
											  editor->draggedCtxHandle);
			vec2 inputPos = GetNodeEditorRectCenter(inputRect);
			ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
			f32 zoomOffset = 1.0f / editor->zoom;
			DrawNodeEditorLine(mouse * zoomOffset, inputPos, 0, vec3(1.0f, 1.0f, 1.0f), 2.0f);
			break;
		}
		case EDITOR_ELEMENT_PARAM: {
			Node *node = GetNode(editor->draggedNode);
			i32 index = editor->draggedCtxHandle + node->inputs.Count();
			Rect paramRect = GetNodeInputRect(editor->draggedNode, index);
			vec2 paramPos = GetNodeEditorRectCenter(paramRect);
			ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
			f32 zoomOffset = 1.0f / editor->zoom;
			DrawNodeEditorLine(mouse * zoomOffset, paramPos, 0, vec3(1.0f, 1.0f, 1.0f), 2.0f);
			break;
		}
		case EDITOR_ELEMENT_OUTPUT: {
			Rect outputRect = GetNodeOutputRect(editor->draggedNode);
			vec2 outputPos = GetNodeEditorRectCenter(outputRect);
			ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
			f32 zoomOffset = 1.0f / editor->zoom;
			DrawNodeEditorLine(mouse * zoomOffset, outputPos, 0, vec3(1.0f, 1.0f, 1.0f), 2.0f);
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
			ImDrawSetColor(vec4(COLOR_BOX_SELECT, 0.3f));
			ImDrawSetNextLayer(2);
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

			float zoomOffset = 1.0f / editor->zoom;

			vec2 a, b;
			a.x = Min(editor->selectDragStart.x, mouse.x);
			a.y = Min(editor->selectDragStart.y, mouse.y);
			b.x = Max(editor->selectDragStart.x, mouse.x);
			b.y = Max(editor->selectDragStart.y, mouse.y);

			a *= zoomOffset;
			b *= zoomOffset;

			for(i32 i = 0; i < _nodeState->nodes.Count(); i++) {
				ObjectHandle handle = _nodeState->nodes.GetHandle(i);
				Node *node = _nodeState->nodes.Get(handle);
				if(node && node->page == GetCurrentPage()) {
					vec2 center = GetNodeEditorRectCenter(node->rect);
					if(center.x > a.x && center.x < b.x &&
					   center.y > a.y && center.y < b.y) {
						editor->selectedNodes.Insert(handle);
					}
				}
			}

			if(editor->selectedNodes.Count() > 1) {
				editor->nodeMultiSelect = true;
			}
		}
	}
}

void UpdateNodeDragging()
{
	NodeEditorState *editor = _nodeEditorState;

	if(!MouseInsideViewerRect()) {
		// start dragging something
		if(mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] && keys_mode == 0 && !editor->isDragging) {
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
	if(singleKeyPress(GLFW_KEY_C) && keys_mode == GLFW_MOD_CONTROL) {
		DebugLog("copying");

		for(i32 i = 0; i < editor->selectedNodes.Count(); i++) {
			Node *node = GetNode(editor->selectedNodes[i]);
			if(node) {
				DebugLog("node:%s", GetStrings()->Get(node->labelHandle)->buffer);
			}
		}
	}
	else if(singleKeyPress(GLFW_KEY_V) && keys_mode == GLFW_MOD_CONTROL) {
		PasteNodes();
		DebugLog("pasting");
	}

	float editorMoveSpeed = 200;
	if(keys[GLFW_KEY_UP]) {
		editor->editorOffset -= ((float)deltaTime * vec2(0, 1)) * editorMoveSpeed;
	}
	else if(keys[GLFW_KEY_DOWN]) {
		editor->editorOffset += ((float)deltaTime * vec2(0, 1)) * editorMoveSpeed;
	}
	if(keys[GLFW_KEY_RIGHT]) {
		editor->editorOffset += ((float)deltaTime * vec2(1, 0)) * editorMoveSpeed;
	}
	else if(keys[GLFW_KEY_LEFT]) {
		editor->editorOffset -= ((float)deltaTime * vec2(1, 0)) * editorMoveSpeed;
	}

	if(mouse_buttons[GLFW_MOUSE_BUTTON_3] ||
	   (mouse_buttons[GLFW_MOUSE_BUTTON_1] && keys_mode == GLFW_MOD_ALT)) {
		float zoomOffset = 1.0f / editor->zoom;
		vec2 dragOffset = mouse - _viewerRenderState->lastDragPos;
		editor->lastEditorDragPos = mouse;

		editor->editorOffset += dragOffset * zoomOffset;

		// if(editor->editorOffset.y > 0.0f)
		// 	editor->editorOffset.y = 0.0f;
		// if(editor->editorOffset.x > 0.0f)
		// 	editor->editorOffset.x = 0.0f;
			
	}


	//////////////////
	// EDITOR ZOOM
	if(scrollReady) {
		editor->zoom += scrollOffset * 0.1;
		if(editor->zoom < 0.2f)
			editor->zoom = 0.2f;

		ImDrawSetFontSize(FONT_SIZE * editor->zoom);
	}

	if(singleKeyPress(GLFW_KEY_EQUAL)) {
		editor->zoom += 0.1;
		if(editor->zoom < 0.2f)
			editor->zoom = 0.2f;

		ImDrawSetFontSize(FONT_SIZE * editor->zoom);
	}
	else if(singleKeyPress(GLFW_KEY_MINUS)) {
		editor->zoom -= 0.1;
		if(editor->zoom < 0.2f)
			editor->zoom = 0.2f;

		ImDrawSetFontSize(FONT_SIZE * editor->zoom);
	}

	//////////////////
	// DRAW NODES
	for(i32 i = 0; i < GetNodes()->Count(); i++) {
		ObjectHandle handle = _nodeState->nodes.GetHandle(i);
		Node *node = GetNodes()->Get(handle);
		if(node && node->page == GetCurrentPage()) {
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


	// DRAW GRID
	i32 lineAmount = 80;
	i32 width, height;
	GetWindowSize(&width, &height);
	ImDrawSetColor(vec3(0.3f, 0.3f, 0.3f));
	ImDrawSetNextLayer(0);

	f32 gridWidth = width * 5;
	// f32 gridHeight = height * 5;

	for(i32 i = 0; i < lineAmount; i++) {
		f32 step = ((gridWidth / lineAmount) * i);
		f32 x = (step + editor->editorOffset.x) * editor->zoom;
		// x = fmod(x, width);
		// f32 x = fmod(step + (editor->editorOffset.x * editor->zoom), width);
		// x = fmod(x, width);
		// if(x < 0.0f)
			// x = width - abs(x);

		vec2 a(x, 0.0f);
		vec2 b(x, height);
		ImDrawLine(a, b);

		f32 y = (step + editor->editorOffset.y) * editor->zoom;
		// f32 y = fmod(step + editor->editorOffset.y, height);
		// if(y < 0.0f)
			// y = height - abs(y);
		vec2 c(0.0f, y);
		vec2 d(width, y);
		ImDrawLine(c, d);
	}

	///////////////////
	// GUI
	NodeGUI();
}

void CleanupNodeEditor()
{
	free(_nodeEditorState);
}
