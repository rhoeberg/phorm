#pragma once

struct GlobalEditorState {
	bool promptActive;
	bool nodeEditorOn;
};

void UpdateGlobalEditor();
void SetPromptActive(bool value);

global GlobalEditorState *_globalEditorState;
