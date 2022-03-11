#pragma once

struct ConsoleMessage
{
	String text;
	vec3 color;

	ConsoleMessage(String _text, vec3 _color)
	{
		text = _text;
		color = _color;
	}
};

enum ConsoleState {
	CONSOLE_HIDE,
	CONSOLE_HALF,
	CONSOLE_FULL,
};

struct Console
{
	PArray<ConsoleMessage> messages;
	bool scrollBottom;
	ConsoleState state;
};

struct GlobalEditorState {
	bool promptActive;
	bool editorFreeze;
	ObjectHandle inspectorObject;
	ObjectHandle viewerNode;
	bool fullscreenView;

	////////////////
	// CURRENT PAGE
	i32 currentPage;
	PArray<ObjectHandle> pages;

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
void ConsoleAddMessage(String text, vec3 color = vec3(1.0f, 1.0f, 1.0f));

global GlobalEditorState *_globalEditorState;
