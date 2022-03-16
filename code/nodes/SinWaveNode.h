#pragma once

void SinWaveOperation(Node *self)
{
	double *output = GetDoubles()->Get(self->GetDataLast());
	if(!output)
		return;

	// PARAMS
	double rate = self->params[0].Double();
	double min = self->params[1].Double();
	double max = self->params[2].Double();

	if(max < min) {
		ErrorLog("SinWaveNode: max cannot be smaller than min");
		return;
	}

	double result = (Sin(GetTime() * rate) + 1.0f) / 2.0f;
	double range = max - min;
	result = (result * range) + min;

	*output = result;

	self->changed = true;
}

void DrawSinWaveNode(Node *self)
{
	SinWaveNodeState *state = _nodeState->sinWaveNodes.Get(self->extraHandle);

	self->rect.width = 150;
	self->rect.height = 70;

	ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
	DrawNodeEditorRect(self->rect, 1, COLOR_NODE_FILL);

	double *output = GetDoubles()->Get(self->GetData());
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
	/* ImDrawText(namePos, self->name); */
	DrawNodeEditorText(namePos, self->name, COLOR_NODE_TEXT);
}

ObjectHandle CreateSinWaveNode()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("rate", 1.0),
		NodeParameter("min", 0.0),
		NodeParameter("max", 1.0),
	};

	FixedArray<NodeInput> inputs = {
	};

	return AddNode(DATA_DOUBLE, params, inputs);
}
