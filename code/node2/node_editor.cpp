#include "node_editor.h"

void InitializeNodeEditor()
{
	_nodeEditorState = (NodeEditorState*)malloc(sizeof(NodeEditorState));
}

void DrawNode(int handle)
{
	TextureNode *node = GetTextureNode(handle);
	Rect rect = {};
	ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
	ImDrawRect(node->rect);
}

void UpdateHoverState()
{
	for(int i = 0; i < _nodeState->textureNodes.Count(); i++) {
		if(PointInsideRect(mouse, _nodeState->textureNodes[i].rect)) {
			_nodeEditorState->hoverState.nodeHandle = i;
			_nodeEditorState->hoverState.hoveringElement = true;
		}
	}
}

void UpdateNodeDragging()
{
	if(_nodeEditorState->hoverState.hoveringElement) {
		if(mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] && !_nodeEditorState->isDragging) {
			_nodeEditorState->draggedNodeHandle = _nodeEditorState->hoverState.nodeHandle;
			_nodeEditorState->isDragging = true;
		}
	}

	if(_nodeEditorState->isDragging) {

		if(!mouse_buttons[GLFW_MOUSE_BUTTON_LEFT]) {
			// handle stopped dragging
			_nodeEditorState->isDragging = false;
		}
		else {
			// handle continued dragging
			_nodeState->textureNodes[_nodeEditorState->draggedNodeHandle].rect.pos = mouse;
		}
	}
}

void UpdateNodeEditor()
{
	//////////////////
	// CHECK HOVERSTATE
	UpdateHoverState();

	UpdateNodeDragging();

	//////////////////
	// DRAW NODES
	for(int i = 0; i < _nodeState->textureNodes.Count(); i++) {
		DrawNode(i);
	}
}

void NodeGUI()
{
	ImGui::Begin("Inspector");
	if(ImGui::Button("add blur node")) {
		AddBlurNode();
	}
	ImGui::End();
}

void CleanupNodeEditor()
{
	free(_nodeEditorState);
}
