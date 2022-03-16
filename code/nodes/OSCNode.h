#pragma once

void OSCNodeOp(Node *self)
{
	vec3 *output = GetVec3s()->Get(self->GetDataLast());
	String *address = GetStrings()->Get(self->params[0].dataHandle);
	if(!address) {
		ErrorLog("OSCNode: could not get string address");
		return;
	}
	*output = OSCTryGetVec3(*address);
}

void DrawOSCNode(Node *self)
{
	self->rect.width = 200;
	self->rect.height = 30;

	DrawNodeEditorRect(self->rect, 1, COLOR_NODE_FILL);

	vec2 namePos = self->rect.pos + vec2(10.0f, self->rect.height - 10.0f);
	DrawNodeEditorText(namePos, self->name, COLOR_NODE_TEXT);

	vec3 *output = GetVec3s()->Get(self->GetDataLast());

	vec2 valuePos = namePos + vec2(70, 0);
	char valueBuf[32];
	sprintf(valueBuf, "%.2f, %.2f, %.2f", output->x, output->y, output->z);

	DrawNodeEditorText(valuePos, valueBuf, COLOR_NODE_TEXT);
}

void SetupOSCNode(Node *self)
{
	self->drawFunc = DrawOSCNode;
}

ObjectHandle CreateOSCNode()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("address", ""),
	};

	FixedArray<NodeInput> inputs = {
	};

	return AddNode(DATA_VEC3, params, inputs);
}
