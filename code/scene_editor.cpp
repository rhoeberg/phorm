#include "scene_editor.h"

void InitializeSceneEditor()
{
	_sceneEditorState = (SceneEditorState*)malloc(sizeof(SceneEditorState));
	new (&_sceneEditorState->scene) Scene();
}

void UpdateSceneEditor()
{
	// shorthand
	SceneEditorState *state = _sceneEditorState;

	ImGui::Begin("Scene Editor");
	ImGui::Text("SCENE");

	for(int i = 0; i < state->scene.objects.Count(); i++) {
		Node *node = GetNode(state->scene.objects[i]);
		String *str = GetString(node->params[1].dataHandle);

		ImGui::Text("%d", i);
        ImGui::SameLine(300); ImGui::Text("%s", str->buffer);
	}

	ImGui::Spacing();

	ImGui::Text("OBJECTS");
	for(int i = 0; i < _nodeState->nodes.Count(); i++) {
		NodeHandle handle = _nodeState->nodes.GetHandle(i);
		Node *node = _nodeState->nodes.Get(handle);
		if(node) {
			if(node->type == DATA_RENDEROBJECT) {
				String *str = GetString(node->params[1].dataHandle);
				ImGui::Text("%s", str->buffer);
				ImGui::SameLine();
				static char buf[32];
				sprintf(buf, "add##%d", i);
				if(ImGui::Button(buf)) {
					state->scene.objects.Insert(handle);
				}
			}
		}
	}

	ImGui::Text("");


	ImGui::End();

	for(int i = 0; i < state->scene.objects.Count(); i++) {
		Node *node = GetNode(state->scene.objects[i]);
		AddToRenderQueue(node->GetData());
	}
}


void CleanupSceneEditor()
{
	_sceneEditorState->scene.Free();
	free(_sceneEditorState);
}
