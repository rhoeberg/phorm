#pragma once

void MeshNoiseOperation(Node *self)
{
	Mesh *input = GetMeshes()->Get(self->inputs[0]);
	if(!input)
		return;

	Mesh *output = GetMeshes()->Get(self->GetDataLast());
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
	}

	// TODO (rhoe) this should be multithreaded or
	//             optimised using compute shader
	for(i32 i = 0; i < input->indices.Count(); i += 3) {
		Vertex a = output->GetVertex(output->indices[i]);
		Vertex b = output->GetVertex(output->indices[i+1]);
		Vertex c = output->GetVertex(output->indices[i+2]);
		vec3 ba = b.Pos() - a.Pos();
		vec3 ca = c.Pos() - a.Pos();
		vec3 cross = glm::cross(ca, ba);
		vec3 normal = glm::normalize(cross);
		output->SetNormalAt(i, normal);
		output->SetNormalAt(i+1, normal);
		output->SetNormalAt(i+2, normal);
	}
}

ObjectHandle CreateMeshNoise(String name, NodeOp op, NodeDrawFunc drawFunc)
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

	return AddNode(name.buffer, DATA_MESH, op, drawFunc, params, inputs);
}
