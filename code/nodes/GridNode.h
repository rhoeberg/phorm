#pragma once

Vertex GetGridVertex(i32 x, i32 y, i32 width, i32 height)
{
	float wStep = 1.0f / width;
	float hStep = 1.0f / height;
	return Vertex((x*wStep)-0.5f, (y*hStep)-0.5f, 0.0f, (x*wStep), (y*hStep), 0.0f, 0.0f, 1.0f);
}

void GridOperation(Node *self)
{
	Mesh *output = GetMeshes()->Get(&self->GetDataLast());
	if(!output)
		return;

	i32 width = self->params[0].i;
	i32 height = self->params[1].i;
	if(width < 1 || height < 1) {
		return;
	}

	output->vertices.Clear();
	output->indices.Clear();

	/* float wStep = 1.0f / width; */
	/* float hStep = 1.0f / height; */

	/* for(i32 x = 0; x < width + 1; x++) { */
	/* 	for(i32 y = 0; y < height + 1; y++) { */
	/* 		float nX = (x * wStep); */
	/* 		float nY = (y * hStep); */
	/* 		float vX = nX - 0.5f; */
	/* 		float vY = nY - 0.5f; */

	/* 		output->vertices.Insert(vX); */
	/* 		output->vertices.Insert(vY); */
	/* 		output->vertices.Insert(0.0f); */

	/* 		output->vertices.Insert(nX); */
	/* 		output->vertices.Insert(nY); */

	/* 		output->vertices.Insert(0.0f); */
	/* 		output->vertices.Insert(0.0f); */
	/* 		output->vertices.Insert(1.0f); */
	/* 	} */
	/* } */

	i32 i = 0;
	for(i32 x = 0; x < width + 1; x++) {
		for(i32 y = 0; y < height + 1; y++) {
			/* i32 step = i + x; */
			if(y < height && x < width) {
				// add quad
				Vertex a = GetGridVertex(x, y, width, height);
				Vertex b = GetGridVertex(x, y+1, width, height);
				Vertex c = GetGridVertex(x+1, y+1, width, height);

				Vertex d = GetGridVertex(x+1, y+1, width, height);
				Vertex e = GetGridVertex(x+1, y, width, height);
				Vertex f = GetGridVertex(x, y, width, height);

				output->AddVertex(a);
				output->AddVertex(b);
				output->AddVertex(c);
				output->AddVertex(d);
				output->AddVertex(e);
				output->AddVertex(f);

				output->indices.Insert(i++);
				output->indices.Insert(i++);
				output->indices.Insert(i++);
				output->indices.Insert(i++);
				output->indices.Insert(i++);
				output->indices.Insert(i++);
			}
		}
	}

	/* i32 i = 0; */
	/* for(i32 x = 0; x < width; x++) { */
	/* 	for(i32 y = 0; y < height; y++) { */
	/* 		i32 step = i + x; */
	/* 		output->indices.Insert(step); */
	/* 		output->indices.Insert(step + 1); */
	/* 		output->indices.Insert(step + height + 1); */

	/* 		output->indices.Insert(step + 1); */
	/* 		output->indices.Insert(step + height + 2); */
	/* 		output->indices.Insert(step + height + 1); */

	/* 		i++; */
	/* 	} */
	/* } */
}

ObjectHandle CreateGridNode(String name, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
		NodeParameter("width", 100),
		NodeParameter("height", 100),
	};

	FixedArray<NodeInput> inputs = {
	};

	return AddNode(name.buffer, DATA_MESH, op, drawFunc, params, inputs);
}
