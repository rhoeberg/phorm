#pragma once

enum ViewerMode {
	VIEW_OBJECT,
	VIEW_SCENE,
};

struct GlobalEditorState {
	bool promptActive;
	bool nodeEditorOn;
	ViewerMode viewerMode;
	ObjectHandle inspectorObject;
};

void UpdateGlobalEditor();
void SetPromptActive(bool value);
void SetInspectorObject(ObjectHandle handle);

global GlobalEditorState *_globalEditorState;
