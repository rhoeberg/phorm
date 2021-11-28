#pragma once

enum NodeEditorElementType {
	EDITOR_ELEMENT_NODE,
	EDITOR_ELEMENT_INPUT,
	EDITOR_ELEMENT_PARAM,
	EDITOR_ELEMENT_OUTPUT,
};

struct NodeHoverState {
	int nodeHandle;
	bool hoveringElement;
	NodeEditorElementType elementType;
	int ctxHandle;
};

struct NodeEditorState {
	NodeHoverState hoverState;

	// create seperate dragstate struct
	bool isDragging;
	int draggedNodeHandle;
	int draggedCtxHandle;
	vec2 dragOffset;
	NodeEditorElementType draggedType;

	// VIEWER STATE
	GLuint textureID;
	GLuint viewerQuad;
	GLuint fboTexture;
	GLuint fbo;
	GLuint viewerShader;
	int viewerWidth, viewerHeight;
};

/* void NodeEditorSetWindowSize(int width, int height); */

global NodeEditorState *_nodeEditorState;
