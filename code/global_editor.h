#pragma once

enum ViewerMode {
	VIEW_OBJECT,
	VIEW_SCENE,
};

struct GlobalEditorState {
	bool promptActive;
	bool editorFreeze;
	ViewerMode viewerMode;
	ObjectHandle inspectorObject;
	ObjectHandle viewerNode;

	////////////////
	// CURRENT PAGE
	i32 currentPage;
	VMArray<ObjectHandle> pages;

	////////////////
	// SCENE RENDER
	bool freeCamMode;
};

void UpdateGlobalEditor();
void EditorFreeze();
void EditorUnfreeze();
void SetPromptActive(bool value);
void SetInspectorObject(ObjectHandle handle);
void SetViewerNode(ObjectHandle handle);
ObjectHandle GetViewerNode();
void SetViewerMode(i32 mode);
u32 GetCurrentPage();
bool FreeCamMode();

global GlobalEditorState *_globalEditorState;
