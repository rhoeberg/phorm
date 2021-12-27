#pragma once

void CubeOperation(Node *self)
{
	Mesh *output = GetMeshes()->Get(&self->GetDataLast());

	output->vertices = {
		// BACK FACE index 0
		// COORDS              UV           NORMAL
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // back, right, bottom
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // back, left, bottom
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // back, left, top
		 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //  back, right, top
       
		// FRONT FACE index 4
		// COORDS              UV           NORMAL
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // front, left, bottom
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f, // front, right, bottom
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f, // front, right, top
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, -1.0f, // front, left, top

		// LEFT FACE index 8..
		// COORDS              UV           NORMAL
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // back, left, bottom
		-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, // front, left, bottom
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f, // front, left, top
		-0.5f, 0.5f, -0.5f,   0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // back, left, top

		// RIGHT FACE index 12..
		// COORDS              UV           NORMAL
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // front, right, bottom
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // back, right, bottom
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // back, right, top
		0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // front, right, top

		// TOP FACE index 16..
		// COORDS              UV           NORMAL
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // front, left, top
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // front, right, top
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, //  back, right, top
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // back, left, top

		// BOTTOM FACE index 20..
		// COORDS              UV           NORMAL
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f, // back, left, bottom
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f, // back, right, bottom
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f, // front, right, bottom
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f, // front, left, bottom
    };

	output->indices = {
		// BACK FACE
		0, 1, 2,
		2, 3, 0,

		// FRONT FACE
		4, 5, 6,
		6, 7, 4,

		// LEFT FACE
		8,  9,  10,
		10, 11, 8,

		// RIGHT FACE
		12, 13, 14,
		14, 15, 12,

		// TOP FACE
		16, 17, 18,
		18, 19, 16,

		// BOTTOM FACE
		20, 21, 22,
		22, 23, 20,
	};
}

ObjectHandle CreateCubeNode(String name, vec2 pos, DataType dataType, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
	};

	FixedArray<NodeInput> inputs = {
	};

	return AddNode(name.buffer, pos, dataType, op, drawFunc, params, inputs);
}
