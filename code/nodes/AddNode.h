#pragma once

void AddOperation(Node *self)
{
	double *input1 = GetDoubles()->Get(self->inputs[0]);
	if(!input1)
		return;

	double value = self->params[0].Double();

	double *output = GetDoubles()->Get(self->GetDataLast());
	*output = *input1 + value;

}

void DrawAddNode(Node *self)
{
	self->rect.width = 150;
	self->rect.height = 30;

	DrawNodeEditorRect(self->rect, 1, COLOR_NODE_FILL);

	vec2 namePos = self->rect.pos + vec2(10.0f, self->rect.height - 10.0f);
	DrawNodeEditorText(namePos, self->name, COLOR_NODE_TEXT);

	vec2 valuePos = namePos + vec2(70, 0);
	char valueBuf[32];
	sprintf(valueBuf, "%.2f", self->params[0].Double());
	DrawNodeEditorText(valuePos, valueBuf, COLOR_NODE_TEXT);
}

void SetupAddNode(Node *self)
{
	self->drawFunc = DrawAddNode;
}

ObjectHandle CreateAddNode()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("value", 1.0),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_DOUBLE),
	};

	return AddNode(DATA_DOUBLE, params, inputs);
}
