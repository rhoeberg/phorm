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

	ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
	ImDrawRect(self->rect);

	vec2 namePos = self->rect.pos + vec2(10.0f, self->rect.height - 10.0f);
	ImDrawText(namePos, self->name);

	vec3 *output = GetVec3s()->Get(self->GetDataLast());

	vec2 valuePos = namePos + vec2(70, 0);
	char valueBuf[32];
	sprintf(valueBuf, "%.2f, %.2f, %.2f", output->x, output->y, output->z);

	ImDrawText(valuePos, valueBuf, vec3(0.3, 0, 0));
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
