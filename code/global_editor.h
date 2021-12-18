#pragma once

struct GlobalEditorState {
	bool promptActive;
	bool nodeEditorOn;
	ObjectHandle inspectorObject;
};

void UpdateGlobalEditor();
void SetPromptActive(bool value);
void SetInspectorObject(ObjectHandle handle);

global GlobalEditorState *_globalEditorState;
