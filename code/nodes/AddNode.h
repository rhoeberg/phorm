#pragma once

void AddOperation(Node *self)
{
	double *input1 = GetDoubles()->Get(&self->inputs[0]);
	if(!input1)
		return;

	double value = self->params[0].Double();

	double *output = GetDoubles()->Get(&self->GetDataLast());
	*output = *input1 + value;

}

void DrawAddNode(Node *self)
{
	self->rect.width = 150;
	self->rect.height = 30;

	ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
	ImDrawRect(self->rect);

	vec2 namePos = self->rect.pos + vec2(10.0f, self->rect.height - 10.0f);
	ImDrawText(namePos, self->name);

	vec2 valuePos = namePos + vec2(70, 0);
	char valueBuf[32];
	sprintf(valueBuf, "%.2f", self->params[0].Double());

	ImDrawText(valuePos, valueBuf, vec3(0.3, 0, 0));
}

ObjectHandle CreateAddNode(String name, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
		NodeParameter("value", 1.0),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_DOUBLE),
	};

	return AddNode(name.buffer, DATA_DOUBLE, op, drawFunc, params, inputs);
}
