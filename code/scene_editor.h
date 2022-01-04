#pragma once

struct SceneEditorState {
	Scene scene;
	ObjectHandle selectedObject;
};

global SceneEditorState *_sceneEditorState;
