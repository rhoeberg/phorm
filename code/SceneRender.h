#pragma once

struct SceneRenderCommand
{
	ObjectHandle scene;
	ObjectHandle texture;
	ObjectHandle sceneRenderData;
};

struct SceneRenderState
{
	VMArray<SceneRenderCommand> commands;
};

void AddSceneRenderCommand(ObjectHandle scene, ObjectHandle texture, ObjectHandle sceneRenderData);
void UpdateSceneRender();
void RenderSceneCommand(SceneRenderCommand command);

global SceneRenderState *_sceneRenderState;

