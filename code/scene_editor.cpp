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

	////////////
	// SCENE OBJECTS
	for(i32 i = 0; i < state->scene.objects.Count(); i++) {
		ObjectHandle handleHandle = state->scene.objects.GetHandle(i);
		ObjectHandle *nodeHandle = state->scene.objects.Get(&handleHandle);
		if(nodeHandle) {
			Node *node = GetNode(nodeHandle);
			if(node) {
				String *str = GetString(&node->params[2].dataHandle);
				if(str) {
					ImGui::Text("%d", i);
					ImGui::SameLine(300);
					if(ImGui::Button(str->buffer)) {
						SetInspectorObject(*nodeHandle);
					}
					// ImGui::Text("%s", str->buffer);
				}
				else {
					ImGui::Text("RENDEROBJECT LABEL NOT FOUND");
				}
			}
		}
	}

	////////////
	// SCENE LIGHTS
	for(i32 i = 0; i < state->scene.pointLights.Count(); i++) {
		ObjectHandle handle = state->scene.pointLights.GetHandle(i);
		ObjectHandle *nodeHandle = state->scene.pointLights.Get(&handle);
		if(nodeHandle) {
			Node *node = GetNode(nodeHandle);
			if(node) {
				String *str = GetString(&node->params[2].dataHandle);
				if(str) {
					ImGui::Text("%d", i);
					ImGui::SameLine(300);
					if(ImGui::Button(str->buffer)) {
						SetInspectorObject(*nodeHandle);
					}
					// ImGui::Text("%s", str->buffer);
				}
				else {
					ImGui::Text("LIGHT LABEL NOT FOUND");
				}

				// char buffer[128];
				// sprintf(buffer, "pos##light%d", i); 
				// // ImGui::InputFloat3(buf, 
				// ImGui::InputFloat3(buffer, glm::value_ptr(light->pos));
				// sprintf(buffer, "col##light%d", i); 
				// ImGui::InputFloat3(buffer, glm::value_ptr(light->color));
			}
		}
	}

	ImGui::Spacing();
	ImGui::Separator();

	//////////
	// ADD STUFF TO SCENE
	ImGui::Text("OBJECTS");

	// if(ImGui::Button("add light")) {
	// 	ObjectHandle handle = state->scene.pointLights.Insert(PointLight());
	// 	PointLight *light = state->scene.pointLights.Get(&handle);
	// 	if(light) {
	// 		light->pos = vec3(1, 1, 1);
	// 		light->color = vec3(1, 1, 1);
	// 	}
	// }

	// add renderobjects
	ImGui::Text("RENDER OBJECTS");
	for(i32 i = 0; i < _nodeState->nodes.Count(); i++) {
		ObjectHandle handle = _nodeState->nodes.GetHandle(i);
		Node *node = _nodeState->nodes.Get(&handle);
		if(node) {
			if(node->type == DATA_RENDEROBJECT) {
				String *str = GetString(&node->params[2].dataHandle);
				if(str) {
					ImGui::Text("%s", str->buffer);
					ImGui::SameLine();
					static char buf[32];
					sprintf(buf, "add##%d", i);
					if(ImGui::Button(buf)) {
						state->scene.objects.Insert(handle);
					}
				}
				else {
					ImGui::Text("RENDEROBJECT LABEL NOT FOUND");
				}
			}
		}
	}

	// add pointlights
	ImGui::Text("LIGHT OBJECTS");
	for(i32 i = 0; i < _nodeState->nodes.Count(); i++) {
		ObjectHandle handle = _nodeState->nodes.GetHandle(i);
		Node *node = _nodeState->nodes.Get(&handle);
		if(node) {
			if(node->type == DATA_POINTLIGHT) {
				String *str = GetString(&node->params[2].dataHandle);
				if(str) {
					ImGui::Text("%s", str->buffer);
					ImGui::SameLine();
					static char buf[32];
					sprintf(buf, "add##%d", i);
					if(ImGui::Button(buf)) {
						state->scene.pointLights.Insert(handle);
					}
				}
				else {
					ImGui::Text("LIGHTOBJECT LABEL NOT FOUND");
				}
			}
		}
	}
	ImGui::End();




	/////////////
	// RENDER SCENE
	for(i32 i = 0; i < state->scene.pointLights.Count(); i++) {
		ObjectHandle handle = state->scene.pointLights.GetHandle(i);
		ObjectHandle *nodeHandle = state->scene.pointLights.Get(&handle);
		if(nodeHandle) {
			Node *node = GetNode(nodeHandle);
			if(node) {
				AddToRenderPointLightQueue(&node->GetData());
			}
			else {
				ObjectHandle handle = state->scene.objects.GetHandle(i);
				state->scene.pointLights.Remove(&handle);
			}
		}
	}

	for(i32 i = 0; i < state->scene.objects.Count(); i++) {
		ObjectHandle handleHandle = state->scene.objects.GetHandle(i);
		ObjectHandle *nodeHandle = state->scene.objects.Get(&handleHandle);
		if(nodeHandle) {
			Node *node = GetNode(nodeHandle);
			if(node) {
				AddToRenderQueue(&node->GetData());
			}
			else {
				ObjectHandle handle = state->scene.objects.GetHandle(i);
				state->scene.objects.Remove(&handle);
			}
		}
	}
}


void CleanupSceneEditor()
{
	_sceneEditorState->scene.Free();
	free(_sceneEditorState);
}
