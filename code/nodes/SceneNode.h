#pragma once

void SceneNodeOp(Node *self)
{
	// this would practically be all we need to do for the scene node
	/* RenderScene(self->extraHandle, self->GetDataLast()); */
}

void SceneNodeEditor(Node *self)
{
	ImGui::Begin("scene node editor");
	ImGui::Text("I AM CUSTOM EDITOR");
	ImGui::End();
}

ObjectHandle CreateSceneNode(String name, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_TEXTURE),
	};

	// extra handle should be scene object

	ObjectHandle handle = AddNode(name.buffer, DATA_TEXTURE, op, drawFunc, params, inputs);
	Node *self = GetNode(handle);
	self->editor = SceneNodeEditor;

	return handle;
}
