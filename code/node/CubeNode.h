#pragma once

void CubeOperation(Node *self)
{
	Mesh *output = GetMesh(self->GetDataLast());

	output->vertices = {
		// BACK FACE
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // back, left, bottom
		0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // back, right, bottom
		0.5f,  0.5f, -0.5f,  0.0f, 1.0f, //  back, right, top
		-0.5f, 0.5f, -0.5f,  1.0f, 0.0f, // back, left, top
       
		// FRONT FACE
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // front, left, bottom
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, //  front, right, bottom
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //  front, right, top
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // front, left, top
    };

	output->indices = {
		// BACK FACE
		0, 1, 2,
		2, 3, 0,

		// FRONT FACE
		4, 5, 6,
		6, 7, 4,

		// LEFT FACE
		0, 4, 7,
		7, 3, 0,

		// RIGHT FACE
		5, 1, 2,
		2, 6, 5,

		// TOP FACE
		7, 6, 2,
		2, 3, 7,

		// BOTTOM FACE
		0, 1, 5,
		5, 4, 0,
	};
    
	// TODO (rhoe) create opengl ids on init
	/* output->VAO = createCubeVAO(); */
	/* output->vertexAmount = 36; */
}

NodeHandle AddCubeNode()
{
	FixedArray<NodeParameter> params = {
	};

	FixedArray<NodeInput> inputs = {
	};

	return AddNode("CUBE", DATA_MESH, OP_MESH_CUBE, params, inputs);
}

