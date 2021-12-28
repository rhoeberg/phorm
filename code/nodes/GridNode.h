#pragma once

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

	float wStep = 1.0f / width;
	float hStep = 1.0f / height;

	for(i32 x = 0; x < width + 1; x++) {
		for(i32 y = 0; y < height + 1; y++) {
			float nX = (x * wStep);
			float nY = (y * hStep);
			float vX = nX - 0.5f;
			float vY = nY - 0.5f;

			output->vertices.Insert(vX);
			output->vertices.Insert(vY);
			output->vertices.Insert(0.0f);

			output->vertices.Insert(nX);
			output->vertices.Insert(nY);

			output->vertices.Insert(0.0f);
			output->vertices.Insert(0.0f);
			output->vertices.Insert(1.0f);
		}
	}

	i32 i = 0;
	for(i32 x = 0; x < width; x++) {
		for(i32 y = 0; y < height; y++) {
			i32 step = i + x;
			output->indices.Insert(step);
			output->indices.Insert(step + 1);
			output->indices.Insert(step + height + 1);

			output->indices.Insert(step + 1);
			output->indices.Insert(step + height + 2);
			output->indices.Insert(step + height + 1);

			i++;
		}
	}
}

ObjectHandle CreateGridNode(String name, vec2 pos, DataType dataType, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
		NodeParameter("width", 100),
		NodeParameter("height", 100),
	};

	FixedArray<NodeInput> inputs = {
	};

	return AddNode(name.buffer, pos, dataType, op, drawFunc, params, inputs);
}
