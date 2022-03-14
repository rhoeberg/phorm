#pragma once

/*
  
Orbits on the XZ plane 

 */
void OrbitOp(Node *self)
{
	vec3 *input = GetVec3s()->Get(self->inputs[0]);
	if(!input) {
		ErrorLog("OrbitNode: could not find input vec3");
	}

	vec3 *output = GetVec3s()->Get(self->GetDataLast());
	if(!output) {
		ErrorLog("OrbitNode: could not find output vec3");
	}

	
	double distance = self->params[0].Double();
	double height = self->params[1].Double();
	double t = self->params[2].Double();

	output->x = input->x + (Sin(t) * distance);
	output->z = input->z + (Cos(t) * distance);
	output->y = input->y + height;
}

ObjectHandle CreateOrbitNode()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("distance", 2.0),
		NodeParameter("height", 0.0),
		NodeParameter("t", 0.0),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_VEC3),
	};

	return AddNode(DATA_VEC3, params, inputs);
}
