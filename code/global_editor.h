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
	ObjectHandle viewerNode;
};

void UpdateGlobalEditor();
void SetPromptActive(bool value);
void SetInspectorObject(ObjectHandle handle);
void SetViewerNode(ObjectHandle handle);
ObjectHandle GetViewerNode();

global GlobalEditorState *_globalEditorState;
