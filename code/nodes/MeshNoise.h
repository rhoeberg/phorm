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
	vec3 offset = self->params[4].Vec3();

	MeshComputeState *state = GetMeshComputeStates()->Get(self->extraHandle);

	// upload input to vga
	/* if(input->vertices.Count() != state->size) { */
		/* state->size = input->vertices.Count(); */
	/* } */

	float *data = input->vertices.Data();
	u64 count = input->vertices.Count() * sizeof(float);
	GFXBufferSetData(state->buffer, count, GFX_DYNAMIC_READ, data);


	state->compute.Begin();
	state->compute.SetUniform("freq", (float)freq);
	state->compute.SetUniform("offset", offset);
	GFXBufferBindBase(state->buffer, 0);

	// TODO (rhoe) maybe we can get better performance by splitting work
	//             into workgroups instead of having workgroups of size 1
	GFXCompute(input->vertices.Count(), 1, 1);
	GFXMemBarrier(GFX_BARRIER_IMAGE);
	state->compute.End();

	float *mappedBuffer = (float*)GFXBufferMap(state->buffer);
	if(mappedBuffer) {
		output->vertices = PArray<float>(input->vertices.Count(), mappedBuffer);
		GFXBufferUnmap(state->buffer);
	}
	
	output->indices = PArray<GLuint>(input->indices);

	/* for(i32 i = 0; i < input->vertices.Count(); i+=8) { */
	/* 	float x = input->vertices[i]; */
	/* 	float y = input->vertices[i+1]; */
	/* 	float z = input->vertices[i+2]; */

	/* 	x += Noise3D(x, y, z, octaves, persistance, freq) * amount; */
	/* 	y += Noise3D(x, y, z, octaves, persistance, freq) * amount; */
	/* 	z += Noise3D(x, y, z, octaves, persistance, freq) * amount; */

	/* 	output->vertices[i] = x; */
	/* 	output->vertices[i+1] = y; */
	/* 	output->vertices[i+2] = z; */
	/* } */

	/* // TODO (rhoe) this should be multithreaded or */
	/* //             optimised using compute shader */
	/* for(i32 i = 0; i < input->indices.Count(); i += 3) { */
	/* 	Vertex a = output->GetVertex(output->indices[i]); */
	/* 	Vertex b = output->GetVertex(output->indices[i+1]); */
	/* 	Vertex c = output->GetVertex(output->indices[i+2]); */
	/* 	vec3 ba = b.Pos() - a.Pos(); */
	/* 	vec3 ca = c.Pos() - a.Pos(); */
	/* 	vec3 cross = glm::cross(ca, ba); */
	/* 	vec3 normal = glm::normalize(cross); */
	/* 	output->SetNormalAt(i, normal); */
	/* 	output->SetNormalAt(i+1, normal); */
	/* 	output->SetNormalAt(i+2, normal); */
	/* } */
}

void SetupMeshNoise(Node *self)
{
	MeshComputeState state = {};
	state.compute = Shader("assets/shaders/meshnoise.comp");
	state.buffer = GFXBufferAdd(GFX_BUFFER_STORAGE);
	state.size = 0;
	self->extraHandle = GetMeshComputeStates()->Insert(state);
}

ObjectHandle CreateMeshNoise()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("amount", 1.0),
		NodeParameter("octaves", 1),
		NodeParameter("persistance", 1.0),
		NodeParameter("freq", 1.0),
		NodeParameter("offsetx", vec3(0, 0, 0)),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_MESH),
	};

	return AddNode(DATA_MESH, params, inputs);
}
