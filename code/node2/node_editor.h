#pragma once

struct NodeHoverState {
	int nodeHandle;
	bool hoveringElement;
};

struct NodeEditorState {
	NodeHoverState hoverState;
	bool isDragging;
	int draggedNodeHandle;
};

global NodeEditorState *_nodeEditorState;
