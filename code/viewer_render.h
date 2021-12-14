#pragma once

#define VIEWER_SIZE 600

struct ViewerRenderState {
	VMArray<ObjectHandle> renderList;
	ObjectHandle baseTextureObject;
	/* RenderObject textureRenderObject; */
	bool wireframe;

	GLuint defaultTexture;

	bool dragging;
	vec2 startDragPos;
	vec2 lastDragPos;
	vec2 dragAmount;
	float orbitDistance;
	float orbitDragSpeed;
};

void InitializeViewerRender();
void UpdateViewerRender();
void AddToRenderQueue(ObjectHandle objectHandle);
void AddTextureToRenderQueue(ObjectHandle textureHandle);

global ViewerRenderState _viewerRenderState;
