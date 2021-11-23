#pragma once

struct NodeHoverState {
	int nodeHandle;
	bool hoveringElement;
};

struct NodeEditorState {
	NodeHoverState hoverState;
	bool isDragging;
	int draggedNodeHandle;

	GLuint textureID;
	GLuint viewerQuad;
	GLuint fboTexture;
	GLuint fbo;
	GLuint viewerShader;
};

global NodeEditorState *_nodeEditorState;
