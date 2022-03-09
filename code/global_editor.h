#pragma once

enum ViewerMode {
	VIEW_OBJECT,
	VIEW_SCENE,
};

struct ConsoleMessage
{
	String text;
	vec4 color;
};

enum ConsoleState {
	CONSOLE_HIDE,
	CONSOLE_HALF,
	CONSOLE_FULL,
};

struct Console
{
	VMArray<String> messages;
	bool scrollBottom;
	ConsoleState state;
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

	////////////////
	// CONSOLE
	Console console;
};

void UpdateGlobalEditor();
void EditorFreeze();
void EditorUnfreeze();
bool EditorIsFrozen();
void SetPromptActive(bool value);
void SetInspectorObject(ObjectHandle handle);
void SetViewerNode(ObjectHandle handle);
ObjectHandle GetViewerNode();
void SetViewerMode(i32 mode);
u32 GetCurrentPage();
bool FreeCamMode();

void ConsoleAddMessage(String msg);

global GlobalEditorState *_globalEditorState;
