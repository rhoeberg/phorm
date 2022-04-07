#include "global_editor.h"
void InitializeGlobalEditor()
{
	_globalEditorState = (GlobalEditorState*)malloc(sizeof(GlobalEditorState));
	_globalEditorState->promptActive = false;
	_globalEditorState->fullscreenView = false;
	_globalEditorState->currentPage = 0;
	_globalEditorState->freeCamMode = true;
	_globalEditorState->editorFreeze = false;
	new(&_globalEditorState->pages) PArray<String>();
	_globalEditorState->pages.Insert(AddString("main"));

	new(&_globalEditorState->console.messages) PArray<ConsoleMessage>();
}

void EditorFreeze()
{
	_globalEditorState->editorFreeze = true;
}

void EditorUnfreeze()
{
	_globalEditorState->editorFreeze = false;
}

bool EditorIsFrozen()
{
	return _globalEditorState->editorFreeze;
}

bool IsPromptActive()
{
	return _globalEditorState->promptActive;
}

void SetPromptActive(bool value)
{
	_globalEditorState->promptActive = value;
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

bool FullscreenViewer()
{
	return _globalEditorState->fullscreenView;
}

///////////////////
// INSPECTOR
///////////////////
void SetInspectorObject(ObjectHandle handle)
{
	if(!_globalEditorState->editorFreeze) {
		_globalEditorState->inspectorObject = handle;
	}
}

void UpdateInspector()
{
	i32 winWidth, winHeight;
	GetWindowSize(&winWidth, &winHeight);
	i32 width = 250;
	i32 height = winHeight / 2;

	ImGui::SetNextWindowPos(ImVec2(winWidth - width, winHeight - height), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(width, height));

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

		sprintf(buffer, "update##update%d", _nodeEditorState->draggedNode.id);
		ImGui::Checkbox(buffer, &node->update);
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
				case DATA_VEC4: {
					if(ImGui::InputFloat4(buffer, glm::value_ptr(param->v4))) {
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

	i32 winWidth, winHeight;
	GetWindowSize(&winWidth, &winHeight);
	i32 width = 250;
	i32 height = winHeight / 3;
	ImGui::SetNextWindowPos(ImVec2(0, winHeight - height), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(width, height));

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
// CONSOLE
///////////////////
void ConsoleAddMessage(String text, vec3 color)
{
	_globalEditorState->console.messages.Insert(ConsoleMessage(text, color));
	_globalEditorState->console.scrollBottom = true;
}

void UpdateConsole()
{
	

	i32 winWidth, winHeight;
	GetWindowSize(&winWidth, &winHeight);
	i32 width = winWidth - (winWidth / 3);

	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoTitleBar;

	i32 fullHeight = 400;
	i32 height = 0;
	switch(_globalEditorState->console.state) {
		case CONSOLE_HIDE: {
			height = 1;
			flags |= ImGuiWindowFlags_NoScrollbar;
			break;
		}
		case CONSOLE_HALF: {
			height = fullHeight / 2;
			break;
		}
		case CONSOLE_FULL: {
			height = fullHeight;
			break;
		}
	}
		

	ImGui::SetNextWindowPos(ImVec2(250, 0), ImGuiCond_Always);

	ImGui::SetNextWindowSize(ImVec2(width, height));

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	bool show = true;
	ImGui::Begin("console", &show, flags);

	if(ImGui::Button("clear")) {
		_globalEditorState->console.messages.Clear();
	}
        
	for(i32 i = 0; i < _globalEditorState->console.messages.Count(); i++) {
		ConsoleMessage msg = _globalEditorState->console.messages[i];
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(msg.color.r, msg.color.g, msg.color.b, 1.0f));
		ImGui::TextWrapped("%s", msg.text.buffer);
		ImGui::PopStyleColor(1);
	}
        
	if (_globalEditorState->console.scrollBottom)
		ImGui::SetScrollHereY(1.0f);
	_globalEditorState->console.scrollBottom = false;
        
	ImGui::End();
	ImGui::PopStyleColor(1);
}

///////////////////
// DEBUG VIEW
///////////////////
void UpdateDebug()
{
	// ImGui::Begin("LABELS");
	// PMap<ObjectHandle> *labels = &_nodeState->labels;
	// HashIter<ObjectHandle> iter = HashIter<ObjectHandle>(labels);

	// HashNode<ObjectHandle> *next = iter.Next();
	// while(next) {
	// 	// we have the next label do something
	// 	ImGui::Text("key:%s\tvalue:%d", next->key.buffer, next->value.id);
	// 	next = iter.Next();
	// }
	// ImGui::End();
	i32 winWidth, winHeight;
	GetWindowSize(&winWidth, &winHeight);
	i32 width = 250;
	i32 height = winHeight / 3;
	ImGui::SetNextWindowPos(ImVec2(0, winHeight - (height * 2)), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(width, height));

	ImGui::Begin("debug");
	if(ImGui::Button("save as")) {
		String path = tinyfd_saveFileDialog(NULL, NULL, 0, NULL, NULL);
		// String path("testsave.octo");

		GetCurrentProject()->projectPath = path;
		// String assetPath(path);
		// assetPath.Concat("/assets");
		GetCurrentProject()->assetPath = String("assets");

		ProjectSave(path);
	}
	// if(ImGui::Button("save")) {
		// ProjectSave(GetCurrentProject()->projectPath);
	// }
	if(ImGui::Button("load")) {
		String path = tinyfd_openFileDialog(NULL, NULL, 0, NULL, NULL, 0);
		// String path("testsave.octo");
		ProjectLoad(path);
	}

	ImGui::Spacing();

	if(ImGui::Button("reset time")) {
		ResetTime();
	}

	ImGui::Text("deltaTime: %f", deltaTime);

	ImGui::Text("CAMERA");
	Camera *cam = &_viewerRenderState->freeCam;
	ImGui::InputFloat3("pos", glm::value_ptr(cam->pos));
	ImGui::InputFloat3("front", glm::value_ptr(cam->front));
	ImGui::Text("speed: %f", cam->moveSpeed);
	ImGui::Text("orbitdist: %f", cam->orbitDist);
	ImGui::Text("magnitude: %f", glm::length(cam->front));

	ImGui::Spacing();
	ImGui::InputFloat2("mouse", glm::value_ptr(mouse));

	ImGui::Spacing();
	ImGui::Text("midi (channel 0, cc 1):%d", _midiState->channels[0].cc[1]);

	ImGui::Spacing();
	ImGui::Text("PROJECT");
	if(ImGui::InputText("assetpath", GetCurrentProject()->assetPath.buffer, GetCurrentProject()->assetPath.bufferSize)) {
		GetCurrentProject()->assetPath.ReCalc();
	}

	ImGui::Spacing();
	ImGui::Text("ZOOM");
	// if(ImGui::InputFloat("zoom", &_nodeEditorState->zoom)) {
	if(ImGui::SliderFloat("zoom", &_nodeEditorState->zoom, 0.2f, 3.0f)) {
		ImDrawSetFontSize(FONT_SIZE * _nodeEditorState->zoom);

		for(i32 i = 0; i < GetNodes()->Count(); i++) {
			ObjectHandle handle = _nodeState->nodes.GetHandle(i);
			Node *node = GetNodes()->Get(handle);
			// node->rect.pos *= _nodeEditorState->zoom;
		}
	}
	
	ImGui::Spacing();
	ImGui::Text("UNDO");
	if(ImGui::Button("undo")) {
		MoveNodeCommand cmd = _commandState->commands[0];
		cmd.Undo();
		// _commandState->commands[0]->Undo();
	}

    ImGui::End();

}

///////////////////
// VIEWER SETTINGS
///////////////////
void UpdateViewerSettings()
{
	i32 winWidth, winHeight;
	GetWindowSize(&winWidth, &winHeight);
	i32 width = 250;
	i32 height = winHeight / 3;
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(width, height));

	ImGui::Begin("viewer/render");
	ImGui::Checkbox("freecam", &_globalEditorState->freeCamMode);

	if(ImGui::Button("viewer")) {
		settings.viewerInMain = !settings.viewerInMain;
		if(settings.viewerInMain) {
			HideViewerOtherWindow();
		}
		else {
			ShowViewerOtherWindow();
		}
	}
	ImGui::End();
}

void UpdateGlobalEditor()
{
	GlobalEditorState *editor = _globalEditorState;

	// GLOBAL HOTKEYS
	if(!editor->promptActive && singleKeyPress(GLFW_KEY_C)) {
		editor->freeCamMode = !editor->freeCamMode;
	}

	if(!editor->promptActive && singleKeyPress(GLFW_KEY_K)) {
		if(editor->console.state == CONSOLE_HIDE)
			editor->console.state = CONSOLE_HALF;
		else if(editor->console.state == CONSOLE_HALF)
			editor->console.state = CONSOLE_FULL;
		else 
			editor->console.state = CONSOLE_HIDE;
	}

	if(!editor->promptActive && singleKeyPress(GLFW_KEY_V)) {
		editor->fullscreenView = !editor->fullscreenView;
	}

	if(!editor->promptActive && singleKeyPress(GLFW_KEY_F)) {
		SetFullscreen(_viewerWindow);
		// editor->fullscreenView = !editor->fullscreenView;
	}
	

	if(!editor->fullscreenView) {
		UpdateNodeEditor();
		UpdateInspector();
		UpdatePageEditor();
		UpdateViewerSettings();
		UpdateConsole();
		UpdateDebug();
	}
}

void CleanupGlobalEditor()
{
	free(_globalEditorState);
}
