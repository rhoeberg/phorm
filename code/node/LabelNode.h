#pragma once

void LabelOperation(Node *self)
{
	
}

void DrawLabelNode(Node *self)
{
	self->rect.width = 200;
	self->rect.height = 30;

	ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
	ImDrawRect(self->rect);

	vec2 namePos = self->rect.pos + vec2(10.0f, self->rect.height - 10.0f);
	ImDrawText(namePos, self->name);

	/* char str[128] = self->params[0].str; */
	vec2 labelPos = namePos + vec2(70, 0);
	/* char labelBuf[32]; */
	/* sprintf(labelBuf, "%s", self->params[0].str); */
	ImDrawText(labelPos, self->params[0].str, vec3(0.3, 0, 0));
}

int AddLabelNode()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("name", DATA_STRING, ""),
	};

	FixedArray<NodeInput> inputs = {
		// TOOD (rhoe) add any type to allow to take any input
		NodeInput(DATA_RENDEROBJECT),
	};

	return AddNode("LABEL", DATA_NONE, OP_LABEL, params, inputs);
}
