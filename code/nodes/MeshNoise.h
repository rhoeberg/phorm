#pragma once

void MeshNoiseOperation(Node *self)
{
	Mesh *input = GetMeshes()->Get(&self->inputs[0]);
	if(!input)
		return;

	Mesh *output = GetMeshes()->Get(&self->GetDataLast());
	if(!output)
		return;

	double amount = self->params[0].Double();
	double octaves = self->params[1].i;
	double persistance = self->params[2].Double();
	double freq = self->params[3].Double();

	output->vertices = VMArray<float>(input->vertices);
	output->indices = VMArray<GLuint>(input->indices);

	for(i32 i = 0; i < input->vertices.Count(); i+=8) {
		float x = input->vertices[i];
		float y = input->vertices[i+1];
		float z = input->vertices[i+2];

		x += Noise3D(x, y, z, octaves, persistance, freq) * amount;
		y += Noise3D(x, y, z, octaves, persistance, freq) * amount;
		z += Noise3D(x, y, z, octaves, persistance, freq) * amount;

		output->vertices[i] = x;
		output->vertices[i+1] = y;
		output->vertices[i+2] = z;

		/* vec3 normal */
	}

	for(i32 i = 0; i < input->indices.Count(); i += 3) {
		float x = output->vertices[output->indices[i]];
		float y = output->vertices[output->indices[i] + 1];
		float z = output->vertices[output->indices[i] + 2];

		float ux = output->vertices[output->indices[i] + 3];
		float uy = output->vertices[output->indices[i] + 4];

		float nx = output->vertices[output->indices[i] + 5];
		float ny = output->vertices[output->indices[i] + 6];
		float nz = output->vertices[output->indices[i] + 7];
		
	}
}

ObjectHandle CreateMeshNoise(String name, vec2 pos, DataType dataType, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
		NodeParameter("amount", 1.0),
		NodeParameter("octaves", 1),
		NodeParameter("persistance", 1.0),
		NodeParameter("freq", 1.0),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_MESH),
	};

	return AddNode(name.buffer, pos, dataType, op, drawFunc, params, inputs);
}
