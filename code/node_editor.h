#pragma once

#define PARAM_WIDTH 30
#define PARAM_HEIGHT 10
#define NODE_HEIGHT 30
#define NODE_BASE_WIDTH 100

enum NodeEditorElementType {
	EDITOR_ELEMENT_NODE,
	EDITOR_ELEMENT_INPUT,
	EDITOR_ELEMENT_PARAM,
	EDITOR_ELEMENT_OUTPUT,
	EDITOR_ELEMENT_NONE,
};

struct NodeHoverState {
	ObjectHandle nodeHandle;
	NodeEditorElementType elementType;
	int ctxHandle;
};

struct NodeEditorState {
	NodeHoverState hoverState;
	/* ObjectHandle viewerNode; */

	////////////////
	// DRAG STATE
	bool isDragging;
	ObjectHandle draggedNode;
	int draggedCtxHandle;
	vec2 dragOffset;
	NodeEditorElementType draggedType;

	////////////////
	// SELECT DRAGGING
	bool nodeMultiSelect;
	vec2 selectDragStart;
	VMArray<ObjectHandle> selectedNodes;

	////////////////
	// ADD NODE PROMPT
	bool promptOpen;
	bool promptSetFocus;
	char promptBuffer[128];
	VMArray<String> promptCandidates;
	int promptCandidateSelected;


	////////////////
	// GLOBAL OUTPUT NODE
	ObjectHandle output;
};

/* void NodeEditorSetWindowSize(int width, int height); */

global NodeEditorState *_nodeEditorState;

void ViewSelectedNode();

ObjectHandle GetOutputHandle();
Node *GetOutputNode();
