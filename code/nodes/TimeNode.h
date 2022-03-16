#pragma once

void TimeOperation(Node *self)
{
	double *output = GetDoubles()->Get(self->GetDataLast());
	*output = GetTime();

	self->changed = true;
}

void DrawTimeNode(Node *self)
{
	self->rect.width = 150;
	self->rect.height = 30;

	/* ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f)); */
	/* ImDrawRect(self->rect); */
	DrawNodeEditorRect(self->rect, 1, COLOR_NODE_FILL);

	vec2 namePos = self->rect.pos + vec2(10.0f, self->rect.height - 10.0f);
	/* ImDrawText(namePos, self->name); */
	DrawNodeEditorText(namePos, self->name, COLOR_NODE_TEXT);

	vec2 timePos = namePos + vec2(70, 0);
	char timeBuf[32];
	sprintf(timeBuf, "%.2f", *GetDoubles()->Get(self->GetDataLast()));
	DrawNodeEditorText(timePos, timeBuf, COLOR_NODE_TEXT);
}

void SetupTimeNode(Node *self)
{
	self->drawFunc = DrawTimeNode;
}

ObjectHandle CreateTimeNode()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("tick", 0.0),
	};

	FixedArray<NodeInput> inputs = {
	};


	return AddNode(DATA_DOUBLE, params, inputs);
}
