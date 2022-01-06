#pragma once

void SawWaveOperation(Node *self)
{
	double *output = GetDoubles()->Get(&self->GetDataLast());
	if(!output)
		return;

	double rate = self->params[0].Double();
	*output = Fmod(GetTime() * rate, 1.0f);

	self->changed = true;
}

void DrawSawWaveNode(Node *self)
{
	SinWaveNodeState *state = _nodeState->sinWaveNodes.Get(&self->extraHandle);

	self->rect.width = 150;
	self->rect.height = 70;

	ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
	ImDrawRect(self->rect);

	double *output = GetDoubles()->Get(&self->GetData());
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

ObjectHandle CreateSawWaveNode(String name, DataType dataType, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
		NodeParameter("rate", 1.0),
	};

	FixedArray<NodeInput> inputs = {
	};

	return AddNode(name.buffer, dataType, op, drawFunc, params, inputs);
}
