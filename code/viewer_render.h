#pragma once

#define VIEWER_SIZE 600

struct ViewerRenderState {
	VMArray<int> renderList;
	int baseTextureObject;
	bool wireframe;
};

void UpdateViewerRender();

global ViewerRenderState _viewerRenderState;
