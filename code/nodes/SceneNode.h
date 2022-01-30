#pragma once

void SceneNodeOp(Node *self)
{
	Scene *scene = GetScenes()->Get(self->extraHandle);
	scene->bgColor = self->params[0].Vec3();

	RenderScene2(self->extraHandle, self->GetDataLast());

	self->changed = true;
}

void SceneNodeEditor(Node *self)
{
	ImGui::Begin("scene editor");

	Scene *scene = GetScenes()->Get(self->extraHandle);

	for(i32 i = 0; i < scene->sceneObjects.Count(); i++) {
		char nameBuffer[128];
		ObjectHandle handle = scene->sceneObjects.GetHandle(i);
		SceneObject *sceneObject = scene->sceneObjects.GetAt(i);
		if(sceneObject) {
			Node *node = GetNode(sceneObject->handle);
			if(node) {
				String *str = GetStrings()->Get(node->labelHandle);
				if(str) {
					ImGui::Text("%d", i);
					ImGui::SameLine(40);
						
					sprintf(nameBuffer, "%s##%d", str->buffer, i);
					if(ImGui::Button(nameBuffer)) {
						/* SetInspectorObject(sceneObject->handle); */
						/* state->selectedObject = handle; */
					}
				}
				else {
					ImGui::Text("RENDEROBJECT LABEL NOT FOUND");
				}
				ImGui::SameLine(150);

				sprintf(nameBuffer, "remove##%d", i);
				if(ImGui::Button(nameBuffer)) {
					scene->sceneObjects.Remove(handle);
				}
			}
		}
	}

	for(i32 i = 0; i < scene->pointLights.Count(); i++) {
		char nameBuffer[128];
		ObjectHandle *handle = scene->pointLights.GetAt(i);
		Node *node = GetNode(*handle);
		if(node) {
			String *str = GetStrings()->Get(node->labelHandle);
			if(str) {
				ImGui::Text("%d", i);
				ImGui::SameLine(40);
						
				sprintf(nameBuffer, "%s##%d", str->buffer, i);
				if(ImGui::Button(nameBuffer)) {
					/* SetInspectorObject(sceneObject->handle); */
					/* state->selectedObject = handle; */
				}
			}

			ImGui::SameLine(40);

			sprintf(nameBuffer, "remove##%d", i);
			if(ImGui::Button(nameBuffer)) {
				scene->pointLights.Remove(*handle);
			}
		}
	}

	ImGui::Spacing();
	ImGui::Separator();

	if(scene) {
		for(i32 i = 0; i < _nodeState->nodes.Count(); i++) {
			ObjectHandle handle = _nodeState->nodes.GetHandle(i);
			Node *node = _nodeState->nodes.Get(handle);
			if(node) {
				String *str = GetStrings()->Get(node->labelHandle);
				if(str) {
					ImGui::Text("%s", str->buffer);
					ImGui::SameLine();
					static char buf[32];
					sprintf(buf, "add##%d", i);
					if(ImGui::Button(buf)) {
						if(node->type == DATA_POINTLIGHT) {
							scene->pointLights.Insert(handle);
						}
						else if(node->type == DATA_RENDEROBJECT) {
							SceneObject sceneObject = SceneObject(handle);
							scene->sceneObjects.Insert(sceneObject);
						}
					}
				}
				else {
					ImGui::Text("RENDEROBJECT LABEL NOT FOUND");
				}
			}
		}
	}
	ImGui::End();
}

void SetupSceneNode(Node *self)
{
	self->editor = SceneNodeEditor;
	self->extraHandle = GetScenes()->Insert(Scene());
}

ObjectHandle CreateSceneNode()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("bgcol", vec3(0.1f, 0.1f, 0.1f)),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_TEXTURE),
	};

	return AddNode(DATA_TEXTURE, params, inputs);
}
