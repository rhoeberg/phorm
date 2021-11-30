#pragma once

#define VIEWER_SIZE 600

struct ViewerRenderState {
	VMArray<DataHandle> renderList;
	DataHandle baseTextureObject;
	bool wireframe;
};

void InitializeViewerRender();
void UpdateViewerRender();
void AddToRenderQueue(DataHandle objectHandle);
void AddTextureToRenderQueue(DataHandle textureHandle);

global ViewerRenderState _viewerRenderState;
