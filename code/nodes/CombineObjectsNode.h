#pragma once

void CombineObjectsOperation(Node *self)
{
	RenderObjectGroup *output = GetRenderObjectGroups()->Get(self->GetDataLast());
	output->pos = self->params[0].Vec3();
	output->scale = self->params[1].Vec3();
	output->rot = self->params[2].Vec3();

	output->renderObjects.Clear();

	Node *inputNode1 = GetNode(self->inputs[0].handle);
	if(inputNode1) {
		ObjectHandle handle = inputNode1->GetData();
		if(handle.dataType == DATA_RENDEROBJECT) {
			RenderObjectInstance instance = RenderObjectInstance(handle);
			output->renderObjects.Insert(instance);
		}
	}

	Node *inputNode2 = GetNode(self->inputs[1].handle);
	if(inputNode2) {
		ObjectHandle handle = inputNode2->GetData();
		if(handle.dataType == DATA_RENDEROBJECT) {
			RenderObjectInstance instance = RenderObjectInstance(handle);
			output->renderObjects.Insert(instance);
		}
	}

}

ObjectHandle CreateCombineObjectsNode(String name, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
		NodeParameter("pos", vec3(0, 0, 0)),
		NodeParameter("scale", vec3(1.0f, 1.0f, 1.0f)),
		NodeParameter("rot", vec3(0.0f, 0.0f, 0.0f)),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_RENDEROBJECT),
		NodeInput(DATA_RENDEROBJECT),
	};

	return AddNode(name.buffer, DATA_RENDEROBJECT_GROUP, op, drawFunc, params, inputs);
}
