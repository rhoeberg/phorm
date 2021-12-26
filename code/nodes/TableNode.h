#pragma once

void TableOperation(Node *self)
{
	double *output = GetDouble(&self->GetDataLast());
	if(!output)
		return;

	i32 *input = GetIntOutput(&self->inputs[0].handle);
	if(!input)
		return;

	double rate = self->params[0].Double();
	*output = Sin(GetTime() * rate);

	self->changed = true;
}

void DrawTabelNode(Node *self)
{
	SinWaveNodeState *state = _nodeState->sinWaveNodes.Get(&self->extraHandle);

	self->rect.width = 150;
	self->rect.height = 70;

	ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
	ImDrawRect(self->rect);

	double *output = GetDouble(&self->GetData());
	if(output) {
		i32 resolution = ARRAY_SIZE(state->histogram);
		float step = self->rect.width / resolution; 
		for(i32 i = 0; i < resolution; i++) {
			Rect sinRect = {};
			sinRect.pos = self->rect.pos + vec2(step * i, self->rect.height / 2.0f);
			sinRect.width = step;
			double offset = self->params[0].Double();
			sinRect.height = (state->histogram[i] - offset) * 10.0f;

			ImDrawSetColor(vec3(0.0f, 0.0f, 0.0f));
			ImDrawRect(sinRect);
		}
	}

	vec2 namePos = self->rect.pos + vec2(10.0f, self->rect.height - 10.0f);
	ImDrawText(namePos, self->name);
}

ObjectHandle CreateTableNode(String name, vec2 pos, DataType dataType, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_INT),
	};



	return AddNode(name.buffer, pos, dataType, op, drawFunc, params, inputs);
}
