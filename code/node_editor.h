#pragma once

#define PARAM_WIDTH 30
#define PARAM_HEIGHT 10
#define NODE_HEIGHT 30
#define NODE_BASE_WIDTH 100

#define COLOR_BG vec3(0.16f, 0.16f, 0.16f)
#define COLOR_NODE_FILL vec3(0.92f, 0.86f, 0.70f)
#define COLOR_NODE_OUTLINE vec3(0.27f, 0.52f, 0.53f)
#define COLOR_NODE_TEXT vec3(0.16f, 0.16f, 0.16f)
#define COLOR_NODE_HIGHLIGHT vec3(0.84f, 0.6f, 0.13f)
#define COLOR_NODE_INPUT vec3(0.27f, 0.52f, 0.53f)
#define COLOR_NODE_OUTPUT vec3(0.8f, 0.14f, 0.11f)
#define COLOR_BOX_SELECT vec3(0.98f, 0.74f, 0.18f)

// dark vec3(0.16f, 0.16f, 0.16f)
// light vec3(0.92f, 0.86f, 0.70f)
// red vec3(0.8f, 0.14f, 0.11f)
// yellow  vec3(0.60f, 0.59f, 0.10f)
// blue vec3(0.27f, 0.52f, 0.53f)
// yellow vec3(0.98f, 0.74f, 0.18f)

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
	// CURRENT PAGE
	i32 currentPage;
	/* VMArray<String> pages; */
	VMArray<ObjectHandle> pages;


	////////////////
	// GLOBAL OUTPUT NODE
	ObjectHandle output;
};

/* void NodeEditorSetWindowSize(int width, int height); */

global NodeEditorState *_nodeEditorState;

void ViewSelectedNode();

u32 GetCurrentPage();
ObjectHandle GetOutputHandle();
Node *GetOutputNode();
