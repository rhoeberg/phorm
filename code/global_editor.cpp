#include "global_editor.h"

void InitializeGlobalEditor()
{
	_globalEditorState = (GlobalEditorState*)malloc(sizeof(GlobalEditorState));
	_globalEditorState->nodeEditorOn = true;
	_globalEditorState->promptActive = false;
	_globalEditorState->viewerMode = VIEW_OBJECT;
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
	if(handle.isset && NodeExists(handle)) {
		Node *node = GetNode(handle);
		switch(node->type) {
			case DATA_TEXTURE: {
				AddTextureToRenderQueue(node->GetData());
				break;
			}
			case DATA_RENDEROBJECT: {
				RenderObjectInstance instance = RenderObjectInstance(node->GetData());
				AddToRenderQueue(instance);
				break;
			}
			case DATA_RENDEROBJECT_GROUP: {
				RenderObjectGroup *group = GetRenderObjectGroups()->Get(node->GetData());
				for(i32 i = 0; i < group->renderObjects.Count(); i++) {
					RenderObjectInstance instance = RenderObjectInstance(node->GetData());
					AddToRenderQueue(instance);
				}
				break;
			}
		}
	}
}

void SetViewerMode(i32 mode)
{
	_globalEditorState->viewerMode = (ViewerMode)mode;
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
	Node *node = GetNode(_globalEditorState->inspectorObject);
	if(node) {

		ImGui::Text("%s", node->name);

		ImGui::Text("node changed: %s", (node->changed) ? "true" : "false");

		if(ImGui::Button("OP")) {
			node->CallOp();
		}

		// label
		char buffer[128];
		sprintf(buffer, "label##label%d", _nodeEditorState->draggedNode.id);
		String *labelStr = GetStrings()->Get(node->labelHandle);
		if(ImGui::InputText(buffer, labelStr->buffer, labelStr->bufferSize)) {
			labelStr->ReCalc();
			// node->changed = true;
		}

		for(int i = 0; i < node->params.Count(); i++) {

			NodeParameter *param = &node->params[i];

			sprintf(buffer, "exposed##exposed%d:%d", _nodeEditorState->draggedNode.id, i);

			// EXPOSE
			ImGui::Checkbox(buffer, &param->exposed);
			ImGui::SameLine();

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
					break;
				}
				case DATA_VEC3: {
					if(ImGui::InputFloat3(buffer, glm::value_ptr(param->v3))) {
						node->changed = true;
					}
					break;
				}
				case DATA_STRING: {
					String *str = GetStrings()->Get(param->dataHandle);
					if(ImGui::InputText(buffer, str->buffer, str->bufferSize)) {
						str->ReCalc();
						node->changed = true;
					}
					break;
				}
			}
		}
	}
	ImGui::End();
}

void CleanupGlobalEditor()
{
	free(_globalEditorState);
}
