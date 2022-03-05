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

	////////////////
	// CURRENT PAGE
	i32 currentPage;
	VMArray<ObjectHandle> pages;
};

void UpdateGlobalEditor();
void SetPromptActive(bool value);
void SetInspectorObject(ObjectHandle handle);
void SetViewerNode(ObjectHandle handle);
ObjectHandle GetViewerNode();
void SetViewerMode(i32 mode);
u32 GetCurrentPage();

global GlobalEditorState *_globalEditorState;
