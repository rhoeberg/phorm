#pragma once

void Vec3NodeOperation(Node *self)
{
	vec3 *output = GetVec3s()->Get(&self->GetDataLast());
	if(!output)
		return;

	output->x = self->params[0].Double();
	output->y = self->params[1].Double();
	output->z = self->params[2].Double();
}

ObjectHandle CreateVec3Node(String name, vec2 pos, DataType dataType, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
		NodeParameter("x", 0.0),
		NodeParameter("y", 0.0),
		NodeParameter("z", 0.0),
	};

	FixedArray<NodeInput> inputs = {
	};

	return AddNode(name.buffer, pos, dataType, op, drawFunc, params, inputs);
}
