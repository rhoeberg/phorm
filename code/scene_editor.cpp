#include "scene_editor.h"

void InitializeSceneEditor()
{
	_sceneEditorState = (SceneEditorState*)calloc(1, sizeof(SceneEditorState));
	new (&_sceneEditorState->scene) Scene();
	_sceneEditorState->selectedObject.isset = false;
}

// void RenderScene()
// {
// 	SceneEditorState *editor = _sceneEditorState;

// 	/////////////
// 	// RENDER SCENE OBJECTS
// 	for(i32 i = 0; i < editor->scene.sceneObjects.Count(); i++) {
// 		SceneObject *sceneObject = editor->scene.sceneObjects.GetAt(i);
// 		if(sceneObject) {
// 			Node *node = GetNode(sceneObject->handle);
// 			if(node) {
// 				switch(node->type) {
// 					case DATA_RENDEROBJECT: {
// 						RenderObjectInstance instance = sceneObject->ToRenderObjectInstance(node->GetData());
// 						AddToRenderQueue(instance);
// 						break;
// 					}
// 					case DATA_RENDEROBJECT_GROUP: {
// 						RenderObjectGroup *group = GetRenderObjectGroups()->Get(node->GetData());
// 						RenderGroupInstance instance = RenderGroupInstance(node->GetData(), group->pos, group->scale, group->rot);
// 						AddToRenderGroupQueue(instance);
// 						break;
// 					}
// 					case DATA_POINTLIGHT: {
// 						RenderLightInstance instance = {};
// 						PointLight *light = GetPointLights()->Get(node->GetData());
// 						instance.pos = sceneObject->pos + light->pos;
// 						instance.color = light->color;
// 						AddToRenderPointLightQueue(instance);
// 						break;
// 					}
// 					default: {

// 						// if its not a renderobject or light just update
// 						node->CallOp();
// 					}
// 				}
// 			}
// 		}
// 	}
// }

SceneObject* GetSelectedSceneObject()
{
	SceneEditorState *state = _sceneEditorState;
	return state->scene.sceneObjects.Get(state->selectedObject);
}

void UpdateSceneEditor()
{
	// shorthand
	SceneEditorState *state = _sceneEditorState;


	ImGui::Begin("Scene Editor");

	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("Scene Objects", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 300), false, window_flags);

		ImGui::Separator();

		////////////
		// SCENE OBJECTS
		for(i32 i = 0; i < state->scene.sceneObjects.Count(); i++) {

			char nameBuffer[128];

			ObjectHandle handle = state->scene.sceneObjects.GetHandle(i);
			SceneObject *sceneObject = state->scene.sceneObjects.GetAt(i);
			if(sceneObject) {
				Node *node = GetNode(sceneObject->handle);
				if(node) {
					String *str = GetStrings()->Get(node->labelHandle);
					if(str) {
						ImGui::Text("%d", i);
						ImGui::SameLine(40);
						
						sprintf(nameBuffer, "%s##%d", str->buffer, i);
						if(ImGui::Button(nameBuffer)) {
							SetInspectorObject(sceneObject->handle);
							state->selectedObject = handle;
						}
					}
					else {
						ImGui::Text("RENDEROBJECT LABEL NOT FOUND");
					}
					ImGui::SameLine(150);

					sprintf(nameBuffer, "remove##%d", i);
					if(ImGui::Button(nameBuffer)) {
						state->scene.sceneObjects.Remove(handle);
					}
				}
			}
		}

		ImGui::Spacing();
		ImGui::Separator();

		//////////
		// ADD STUFF TO SCENE
		ImGui::Text("OBJECTS");

		// add renderobjects
		ImGui::Text("RENDER OBJECTS");
		for(i32 i = 0; i < _nodeState->nodes.Count(); i++) {
			ObjectHandle handle = _nodeState->nodes.GetHandle(i);
			Node *node = _nodeState->nodes.Get(handle);
			if(node) {
				// if(node->type == DATA_RENDEROBJECT || node->type == DATA_RENDEROBJECT_GROUP || node->type == DATA_POINTLIGHT) {
					String *str = GetStrings()->Get(node->labelHandle);
					if(str) {
						ImGui::Text("%s", str->buffer);
						ImGui::SameLine();
						static char buf[32];
						sprintf(buf, "add##%d", i);
						if(ImGui::Button(buf)) {
							SceneObject sceneObject = SceneObject(handle);
							state->scene.sceneObjects.Insert(sceneObject);
						}
					}
					else {
						ImGui::Text("RENDEROBJECT LABEL NOT FOUND");
					}
				// }
			}
		}

		ImGui::EndChild();
	}

	ImGui::SameLine();

	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("SceneObject Inspector", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 300), false, window_flags);

		ImGui::Separator();

		ImGui::Text("SceneObject Inspector");

		ImGui::EndChild();
	}

	ImGui::End();
}


void CleanupSceneEditor()
{
	_sceneEditorState->scene.Free();
	free(_sceneEditorState);
}
