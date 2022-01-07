#include "scene_editor.h"

void InitializeSceneEditor()
{
	_sceneEditorState = (SceneEditorState*)calloc(1, sizeof(SceneEditorState));
	new (&_sceneEditorState->scene) Scene();
	_sceneEditorState->selectedObject.isset = false;
}

void RenderScene()
{
	SceneEditorState *editor = _sceneEditorState;

	/////////////
	// RENDER SCENE LIGHTS
	// for(i32 i = 0; i < editor->scene.pointLights.Count(); i++) {
	// 	ObjectHandle handle = editor->scene.pointLights.GetHandle(i);
	// 	ObjectHandle *nodeHandle = editor->scene.pointLights.Get(&handle);
	// 	if(nodeHandle) {
	// 		Node *node = GetNode(nodeHandle);
	// 		if(node) {
	// 			AddToRenderPointLightQueue(&node->GetData());
	// 		}
	// 		else {
	// 			editor->scene.pointLights.Remove(&handle);
	// 		}
	// 	}
	// }

	/////////////
	// RENDER SCENE OBJECTS
	for(i32 i = 0; i < editor->scene.sceneObjects.Count(); i++) {
		SceneObject *sceneObject = editor->scene.sceneObjects.GetAt(i);
		if(sceneObject) {
			Node *node = GetNode(sceneObject->handle);
			if(node) {
				if(node->type == DATA_RENDEROBJECT) {
					RenderObjectInstance instance = sceneObject->ToRenderObjectInstance(node->GetData());
					AddToRenderQueue(instance);
				}
				else if(node->type == DATA_RENDEROBJECT_GROUP) {
					RenderObjectGroup *group = GetRenderObjectGroups()->Get(node->GetData());
					RenderGroupInstance instance = RenderGroupInstance(node->GetData(), group->pos, group->scale, group->rot);
					AddToRenderGroupQueue(instance);
					// for(i32 j = 0; j < group->renderObjects.Count(); j++) {
					// 	SceneObject nestedSceneObject = group->renderObjects[j];
					// 	Node *nestedNode = GetNode(&nestedSceneObject.handle);
					// 	RenderObjectInstance instance = RenderObjectInstance(nestedNode->GetData(), nestedSceneObject.pos, nestedSceneObject.scale, nestedSceneObject.rot);
					// 	AddToRenderQueue(instance);
					// }
				}
				else if(node->type == DATA_POINTLIGHT) {
					RenderLightInstance instance = {};
					PointLight *light = GetPointLights()->Get(node->GetData());
					instance.pos = sceneObject->pos + light->pos;
					instance.color = light->color;
					AddToRenderPointLightQueue(instance);
				}
			}
		}
	}

	// /////////////
	// // RENDER SCENE GROUPS
	// for(i32 i = 0; i < editor->scene.groupInstances.Count(); i++) {
	// 	RenderGroupInstance *instance = editor->scene.groupInstances.GetAt(i);
	// 	if(instance) {
	// 		if(instance->dataHandle.dataType == DATA_RENDEROBJECT_GROUP) {
	// 		 	// handle renderobject group
	// 			RenderObjectGroup *group = GetRenderObjectGroups()->Get(&instance->dataHandle);
	// 			for(i32 i = 0; i < group->renderObjects.Count(); i++) {
	// 				AddToRenderQueue(group->renderObjects[i]);
	// 			}
	// 		}
	// 	}
	// }
}

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

		////////////
		// SCENE LIGHTS
		// for(i32 i = 0; i < state->scene.pointLights.Count(); i++) {
		// 	ObjectHandle handle = state->scene.pointLights.GetHandle(i);
		// 	ObjectHandle *nodeHandle = state->scene.pointLights.Get(&handle);
		// 	if(nodeHandle) {
		// 		Node *node = GetNode(nodeHandle);
		// 		if(node) {
		// 			String *str = GetStrings()->Get(&node->labelHandle);
		// 			if(str) {
		// 				ImGui::Text("%d", i);
		// 				ImGui::SameLine(40);
		// 				if(ImGui::Button(str->buffer)) {
		// 					SetInspectorObject(*nodeHandle);
		// 				}
		// 				// ImGui::Text("%s", str->buffer);
		// 			}
		// 			else {
		// 				ImGui::Text("LIGHT LABEL NOT FOUND");
		// 			}
		// 			ImGui::SameLine(150);
		// 			if(ImGui::Button("remove")) {
		// 				state->scene.pointLights.Remove(&handle);
		// 			}

		// 			// char buffer[128];
		// 			// sprintf(buffer, "pos##light%d", i); 
		// 			// // ImGui::InputFloat3(buf, 
		// 			// ImGui::InputFloat3(buffer, glm::value_ptr(light->pos));
		// 			// sprintf(buffer, "col##light%d", i); 
		// 			// ImGui::InputFloat3(buffer, glm::value_ptr(light->color));
		// 		}
		// 	}
		// }

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
				if(node->type == DATA_RENDEROBJECT || node->type == DATA_RENDEROBJECT_GROUP || node->type == DATA_POINTLIGHT) {
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
				}
				// else if(node->type == DATA_RENDEROBJECT_GROUP) {
				// 	String *str = GetStrings()->Get(&node->labelHandle);
				// 	if(str) {
				// 		ImGui::Text("%s", str->buffer);
				// 		ImGui::SameLine();
				// 		static char buf[32];
				// 		sprintf(buf, "add##%d", i);
				// 		if(ImGui::Button(buf)) {
				// 			RenderGroupInstance instance = RenderGroupInstance(handle);
				// 			state->scene.groupInstances.Insert(instance);
				// 		}
				// 	}
				// 	else {
				// 		ImGui::Text("RENDERGROUP LABEL NOT FOUND");
				// 	}

				// }
			}
		}

		// add pointlights
		// ImGui::Text("LIGHT OBJECTS");
		// for(i32 i = 0; i < _nodeState->nodes.Count(); i++) {
		// 	ObjectHandle handle = _nodeState->nodes.GetHandle(i);
		// 	Node *node = _nodeState->nodes.Get(&handle);
		// 	if(node) {
		// 		if(node->type == DATA_POINTLIGHT) {
		// 			String *str = GetStrings()->Get(&node->labelHandle);
		// 			if(str) {
		// 				ImGui::Text("%s", str->buffer);
		// 				ImGui::SameLine();
		// 				static char buf[32];
		// 				sprintf(buf, "add##%d", i);
		// 				if(ImGui::Button(buf)) {
		// 					state->scene.pointLights.Insert(handle);
		// 				}
		// 			}
		// 			else {
		// 				ImGui::Text("LIGHTOBJECT LABEL NOT FOUND");
		// 			}
		// 		}
		// 	}
		// }
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
