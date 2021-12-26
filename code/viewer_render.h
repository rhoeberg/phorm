#pragma once

#define VIEWER_SIZE 600

struct ViewerRenderState {
	VMArray<ObjectHandle> renderList;
	VMArray<ObjectHandle> renderPointLights;
	ObjectHandle baseTextureObject;
	/* RenderObject textureRenderObject; */
	bool wireframe;

	GLuint defaultTexture;
	GLuint defaultShader;

	// FRAMEBUFFER FOR MAIN WINDOW VIEWER
	GLuint fbo;
	GLuint fboTexture;
	GLuint quad;

	// CAMERA
	bool dragging;
	vec2 startDragPos;
	vec2 lastDragPos;
	vec2 dragAmount;
	float orbitDistance;
	float orbitDragSpeed;
};

void InitializeViewerRender();
void UpdateViewerRender();
void AddToRenderQueue(ObjectHandle *objectHandle);
void AddTextureToRenderQueue(ObjectHandle *textureHandle);
void AddToRenderPointLightQueue(ObjectHandle *handle);

global ViewerRenderState _viewerRenderState;
