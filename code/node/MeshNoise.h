#pragma once

void MeshNoiseOperation(Node *self)
{
	Mesh *input = GetMeshInput(self->inputs[0]);
	if(!input)
		return;

	Mesh *output = GetMesh(&self->GetDataLast());
	if(!output)
		return;

	double amount = self->params[0].Double();
	double octaves = self->params[1].i;
	double persistance = self->params[2].Double();
	double freq = self->params[3].Double();

	output->vertices = VMArray<float>(input->vertices);
	output->indices = VMArray<GLuint>(input->indices);

	for(i32 i = 0; i < input->vertices.Count(); i+=5) {
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
}

/* ObjectHandle AddMeshNoise() */
/* { */
/* 	FixedArray<NodeParameter> params = { */
/* 		NodeParameter("amount", 1.0), */
/* 		NodeParameter("octaves", 1), */
/* 		NodeParameter("persistance", 1.0), */
/* 		NodeParameter("freq", 1.0), */
/* 	}; */

/* 	FixedArray<NodeInput> inputs = { */
/* 		NodeInput(DATA_MESH), */
/* 	}; */

/* 	return AddNode("MESH NOISE", DATA_MESH, OP_MESH_NOISE, params, inputs); */
/* } */
