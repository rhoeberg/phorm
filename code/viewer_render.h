#pragma once

#define VIEWER_SIZE 600

struct ViewerRenderState {
	VMArray<DataHandle> renderList;
	DataHandle baseTextureObject;
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
void AddToRenderQueue(DataHandle objectHandle);
void AddTextureToRenderQueue(DataHandle textureHandle);

global ViewerRenderState _viewerRenderState;
