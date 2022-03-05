#include "global_editor.h"

void InitializeGlobalEditor()
{
	_globalEditorState = (GlobalEditorState*)malloc(sizeof(GlobalEditorState));
	_globalEditorState->nodeEditorOn = true;
	_globalEditorState->promptActive = false;
	_globalEditorState->viewerMode = VIEW_OBJECT;
	_globalEditorState->currentPage = 0;
	_globalEditorState->freeCamMode = true;
	new(&_globalEditorState->pages) VMArray<String>();
	_globalEditorState->pages.Insert(AddString("main"));
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

bool FreeCamMode()
{
	return _globalEditorState->freeCamMode;
}

void RenderViewerNode()
{
	// ObjectHandle handle = GetViewerNode();
	// if(handle.isset && NodeExists(handle)) {
	// 	Node *node = GetNode(handle);
	// 	switch(node->type) {
	// 		case DATA_TEXTURE: {
	// 			AddTextureToRenderQueue(node->GetData());
	// 			break;
	// 		}
	// 		case DATA_RENDEROBJECT: {
	// 			RenderObjectInstance instance = RenderObjectInstance(node->GetData());
	// 			AddToRenderQueue(instance);
	// 			break;
	// 		}
	// 		case DATA_RENDEROBJECT_GROUP: {
	// 			RenderObjectGroup *group = GetRenderObjectGroups()->Get(node->GetData());
	// 			for(i32 i = 0; i < group->renderObjects.Count(); i++) {
	// 				RenderObjectInstance instance = RenderObjectInstance(node->GetData());
	// 				AddToRenderQueue(instance);
	// 			}
	// 			break;
	// 		}
	// 		case DATA_SCENE: {
	// 			Scene *scene = GetScenes()->Get(node->GetData());
	// 			if(scene) {
	// 				for(i32 i = 0; i < scene->sceneObjects.Count(); i++) {
	// 					SceneObject *sceneObj = scene->sceneObjects.GetAt(i);
	// 					if(sceneObj) {
	// 						Node *node = GetNode(sceneObj->handle);
	// 						RenderObjectInstance instance = RenderObjectInstance(node->GetData());
	// 						AddToRenderQueue(instance);
	// 					}
	// 				}
	// 			}
	// 		}
	// 		default: {
	// 			// TODO (rhoe) trying to show node type with no view 
	// 		}
	// 	}
	// }
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

void UpdateInspector()
{
	ImGui::Begin("Inspector");
	Node *node = GetNode(_globalEditorState->inspectorObject);
	if(node) {

		ImGui::Text("%s", node->name);

		if(node->editor != NULL) {
			node->editor(node);
		}

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

		ImGui::Spacing();

		sprintf(buffer, "page##page%d", _nodeEditorState->draggedNode.id);
		ImGui::InputInt(buffer, &node->page);

		ImGui::Spacing();

		ImGui::Text("output");
		switch(node->type) {
			case DATA_INT: {
				ImGui::Text("value: %d", *GetInts()->Get(node->GetDataLast()));
				break;
			}
			case DATA_DOUBLE: {
				ImGui::Text("value: %f", *GetDoubles()->Get(node->GetDataLast()));
				break;
			}
			default: {
				ImGui::Text("no value inspector implemented for output type");
				break;
			}
		}

		ImGui::Spacing();

		for(int i = 0; i < node->params.Count(); i++) {
			NodeParameter *param = &node->params[i];

			sprintf(buffer, "exposed##exposed%d:%d", _nodeEditorState->draggedNode.id, i);

			// EXPOSE
			ImGui::Checkbox(buffer, &param->exposed);
			ImGui::SameLine();

			ImGui::Text("params");
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
				default: {
					NOT_IMPLEMENTED
				}
			}
		}
	}
	ImGui::End();
}


///////////////////
// PAGE EDITOR
///////////////////
u32 GetCurrentPage()
{
	return _globalEditorState->currentPage;
}

void UpdatePageEditor()
{
	GlobalEditorState *editor = _globalEditorState;

	ImGui::Begin("editor");

	// ImGui::InputInt("page", &editor->currentPage);

	if(ImGui::Button("add page")) {
		editor->pages.Insert(AddString(""));
	}

	char buffer[128];
	for(i32 i = 0; i < editor->pages.Count(); i++) {
		sprintf(buffer, "%d:", i);
		if(ImGui::Button(buffer)) {
			editor->currentPage = i;
		}
		ImGui::SameLine();
		String *pageName = GetStrings()->Get(editor->pages[i]);
		if(pageName) {
			if(i != 0) {
				sprintf(buffer, "##pagename%d", i);
				if(ImGui::InputText(buffer, pageName->buffer, pageName->bufferSize)) {
					pageName->ReCalc();
				}
			}
			else {
				ImGui::Text(pageName->buffer);
			}
		}
	}


	ImGui::End();
}

///////////////////
// VIEWER SETTINGS
///////////////////
void UpdateViewerSettings()
{
	ImGui::Begin("viewer/render");
	ImGui::Checkbox("freecam", &_globalEditorState->freeCamMode);
	ImGui::End();
}

void UpdateGlobalEditor()
{
	GlobalEditorState *editor = _globalEditorState;

	// SEND TO RENDER
	// TOOD (rhoe) this should probably be moved to viewer
	switch(editor->viewerMode) {
		case VIEW_OBJECT: {
			RenderViewerNode();
			break;
		}
		case VIEW_SCENE: {
			// RenderScene();
			break;
		}
	}

	// GLOBAL HOTKEYS
	// if(!_globalEditorState->promptActive && singleKeyPress(GLFW_KEY_V))
	// 	_globalEditorState->nodeEditorOn = !_globalEditorState->nodeEditorOn;
	if(!editor->promptActive && singleKeyPress(GLFW_KEY_C)) {
		editor->freeCamMode = !editor->freeCamMode;
	}

	if(editor->nodeEditorOn) {
		UpdateNodeEditor();
	}
	else {
		UpdateSceneEditor();
	}

	UpdateInspector();
	UpdatePageEditor();
	UpdateViewerSettings();
}

void CleanupGlobalEditor()
{
	free(_globalEditorState);
}
