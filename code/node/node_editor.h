#pragma once

#define PARAM_WIDTH 30
#define PARAM_HEIGHT 17
#define NODE_HEIGHT 28
#define NODE_BASE_WIDTH 100

enum NodeEditorElementType {
	EDITOR_ELEMENT_NODE,
	EDITOR_ELEMENT_INPUT,
	EDITOR_ELEMENT_PARAM,
	EDITOR_ELEMENT_OUTPUT,
};

struct NodeHoverState {
	ObjectHandle nodeHandle;
	bool hoveringElement;
	NodeEditorElementType elementType;
	int ctxHandle;
};

struct NodeEditorState {
	NodeHoverState hoverState;

	// create seperate dragstate struct
	bool isDragging;
	ObjectHandle draggedNode;
	int draggedCtxHandle;
	vec2 dragOffset;
	NodeEditorElementType draggedType;
	ObjectHandle selectedNode;

	// ADD NODE PROMPT
	bool promptOpen;
	bool promptSetFocus;
	char promptBuffer[128];
	VMArray<String> promptCandidates;
	int promptCandidateSelected;
};

/* void NodeEditorSetWindowSize(int width, int height); */

global NodeEditorState *_nodeEditorState;
