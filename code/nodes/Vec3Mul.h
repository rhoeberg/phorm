#pragma once

void Vec3MulOp(Node *self)
{
	vec3 multiplier = self->params[0].Vec3();

	vec3 *input = GetVec3s()->Get(self->inputs[0]);
	if(!input) {
		ErrorLog("Vec3Mul: could not find input");
	}
	vec3 *output = GetVec3s()->Get(self->GetDataLast());
	if(!output) {
		ErrorLog("Vec3Mul: could not find output");
	}

	*output = *input * multiplier;
}

ObjectHandle CreateVec3Mul()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("multiplier", vec3(1, 1, 1)),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_VEC3),
	};

	return AddNode(DATA_VEC3, params, inputs);
}
