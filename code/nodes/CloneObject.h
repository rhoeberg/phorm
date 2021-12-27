#pragma once

void CloneObjectOperation(Node *self)
{
	RenderObjectGroup *output = GetRenderObjectGroups()->Get(&self->GetDataLast());

	output->renderObjects.Clear();

	for(i32 x = 0;

	Node *inputNode1 = GetNode(&self->inputs[0].handle);
	if(inputNode1) {
		ObjectHandle handle = inputNode1->GetData();
		if(handle.dataType == DATA_RENDEROBJECT) {
			output->renderObjects.Insert(handle);
		}
	}

	Node *inputNode2 = GetNode(&self->inputs[1].handle);
	if(inputNode2) {
		ObjectHandle handle = inputNode2->GetData();
		if(handle.dataType == DATA_RENDEROBJECT) {
			output->renderObjects.Insert(handle);
		}
	}

}

ObjectHandle CreateCloneObjectNode(String name, vec2 pos, DataType dataType, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
		NodeParameter("x", 10);
		NodeParameter("y", 10);
		NodeParameter("z", 10);
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_RENDEROBJECT),
	};

	return AddNode(name.buffer, pos, dataType, op, drawFunc, params, inputs);
}
