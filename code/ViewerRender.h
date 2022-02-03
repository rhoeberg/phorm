#pragma once

struct ViewerRenderState
{
	i32 quadVAO;
	Shader shader;
	u32 outputTextureID;


	SceneRenderData sceneRenderData;
	Scene defaultScene;

	Camera freeCam;
	vec2 lastDragPos;

	u32 currentViewTextureID;
};

global ViewerRenderState *_viewerRenderState;

void UpdateViewerRender();
void RenderView(i32 x, i32 y, i32 width, i32 height);
