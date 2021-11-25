#pragma once

struct NodeHoverState {
	int nodeHandle;
	bool hoveringElement;
};

struct NodeEditorState {
	NodeHoverState hoverState;

	// create seperate dragstate struct
	bool isDragging;
	int draggedNodeHandle;
	vec2 dragOffset;

	GLuint textureID;
	GLuint viewerQuad;
	GLuint fboTexture;
	GLuint fbo;
	GLuint viewerShader;
};

global NodeEditorState *_nodeEditorState;
