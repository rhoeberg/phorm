#include "global_editor.h"

void InitializeGlobalEditor()
{
	_globalEditorState = (GlobalEditorState*)malloc(sizeof(GlobalEditorState));
	_globalEditorState->nodeEditorOn = true;
	_globalEditorState->promptActive = false;
}

bool IsPromptActive()
{
	return _globalEditorState->promptActive;
}

void SetPromptActive(bool value)
{
	_globalEditorState->promptActive = value;
}

void SetInspectorObject(ObjectHandle handle)
{
	_globalEditorState->inspectorObject = handle;
}

void SetViewerNode(ObjectHandle handle)
{
	_globalEditorState->viewerNode = handle;
}

ObjectHandle GetViewerNode()
{
	return _globalEditorState->viewerNode;
}

void RenderViewerNode()
{
	ObjectHandle handle = GetViewerNode();
	if(handle.isset && NodeExists(&handle)) {
		Node *node = GetNode(&handle);
		switch(node->type) {
			case DATA_TEXTURE: {
				AddTextureToRenderQueue(&node->GetData());
				break;
			}
			case DATA_RENDEROBJECT: {
				AddToRenderQueue(&node->GetData());
				break;
			}
		}
	}
}


void ToggleViewerMode()
{
	if(_globalEditorState->viewerMode == VIEW_OBJECT) {
		_globalEditorState->viewerMode = VIEW_SCENE;
	}
	else {
		_globalEditorState->viewerMode = VIEW_OBJECT;
	}
}

void UpdateGlobalEditor()
{
	// GLOBAL HOTKEYS
	if(!_globalEditorState->promptActive && singleKeyPress(GLFW_KEY_V))
		_globalEditorState->nodeEditorOn = !_globalEditorState->nodeEditorOn;

	if(_globalEditorState->nodeEditorOn) {
		UpdateNodeEditor();
	}
	else {
		UpdateSceneEditor();
	}

	ImGui::Begin("Inspector");
	// if(_nodeEditorState->draggedNode.isset) {
	Node *node = GetNode(&_globalEditorState->inspectorObject);
	if(node) {

		ImGui::Text("%s", node->name);

		ImGui::Text("node changed: %s", (node->changed) ? "true" : "false");

		if(ImGui::Button("OP")) {
			node->CallOp();
		}

		for(int i = 0; i < node->params.Count(); i++) {

			NodeParameter *param = &node->params[i];

			// EXPOSE
			ImGui::Checkbox("##exposed", &param->exposed);
			ImGui::SameLine();

			// TODO (rhoe) be carefull about imgui widget names
			//             they are used as id's and have to be unique
			//             maybe we need to use the imgui hidden name id feature
			//             where we add the id of the object somehow
			char buffer[128];
			sprintf(buffer, "%s##%d", &param->name, _nodeEditorState->draggedNode.id);

			switch(node->params[i].type) {
				case DATA_INT: {
					if(ImGui::InputInt(buffer, &param->i)) {
						node->changed = true;
					}
					break;
				}
				case DATA_DOUBLE: {
					if(ImGui::InputDouble(buffer, &node->params[i].d)) {
						node->changed = true;
					}
					// if(ImGui::SliderScalar(buffer, ImGuiDataType_Double, &param->d, &param->d_min, &param->d_max)) {
					// node->changed = true;
					// }
					break;
				}
				case DATA_VEC3: {
					if(ImGui::InputFloat3(buffer, glm::value_ptr(param->v3))) {
						node->changed = true;
					}
					break;
				}
				case DATA_STRING: {
					// static char buf[128];
					// ImGui::InputText(buffer, buf, ARRAY_SIZE(buf));
					String *str = GetString(&param->dataHandle);
					if(ImGui::InputText(buffer, str->buffer, str->bufferSize)) {
						str->ReCalc();
						node->changed = true;
					}
					break;
				}
			}
		}
	}
	// }
	ImGui::End();


	// SEND TO RENDER
	// TOOD (rhoe) this should probably be moved to viewer
	switch(_globalEditorState->viewerMode) {
		case VIEW_OBJECT: {
			RenderViewerNode();
			break;
		}
		case VIEW_SCENE: {
			RenderScene();
			break;
		}
	}

}

void CleanupGlobalEditor()
{
	free(_globalEditorState);
}
