#pragma once

void MirrorOperation(Node *self)
{
	RenderObject *output = GetRenderObjects()->Get(&self->GetDataLast());
	RenderObject *input = GetRenderObjects()->Get(&self->inputs[0]);
	if(!input)
		return;

	*output = *input;

	if(self->params[0].i == 1) {
		output->pos.x = -1.0f * input->pos.x;
	}
	if(self->params[1].i == 1) {
		output->pos.y = -1.0f * input->pos.y;
	}
	if(self->params[2].i == 1) {
		output->pos.z = -1.0f * input->pos.z;
	}

	output->rot = -1.0f * input->rot;

	output->scale = input->scale;
}

ObjectHandle CreateMirrorNode(String name, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
		NodeParameter("y", 1),
		NodeParameter("x", 1),
		NodeParameter("z", 1),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_RENDEROBJECT),
	};

	return AddNode(name.buffer, DATA_RENDEROBJECT, op, drawFunc, params, inputs);
}
