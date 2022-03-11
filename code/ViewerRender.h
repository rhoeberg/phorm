#pragma once

struct ViewerRenderState
{
	i32 quadVAO;
	Shader shader;
	Shader objectShader;
	u32 outputTextureID;

	ObjectHandle sceneRenderData;
	Scene defaultScene;

	Camera freeCam;
	vec2 lastDragPos;

	u32 currentViewTextureID;

	ObjectHandle baseTextureObject;
};

global ViewerRenderState *_viewerRenderState;

void UpdateViewerRender();
void RenderView();
