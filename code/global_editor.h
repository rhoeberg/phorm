#pragma once

struct GlobalEditorState {
	bool promptActive;
};

void UpdateGlobalEditor();

global GlobalEditorState *_globalEditorState;
