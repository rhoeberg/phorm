#pragma once

#define OPERATION(name) 


void TimeOperation(Node *self)
{
	double *output = GetDouble(self->GetDataLast());
	*output = glfwGetTime();
	char testtext = "test text";
}

void DrawTimeNode(Node *self)
{
	self->rect.width = 150;
	self->rect.height = 30;

	ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
	ImDrawRect(self->rect);

	vec2 namePos = self->rect.pos + vec2(10.0f, self->rect.height - 10.0f);
	ImDrawText(namePos, self->name);

	vec2 timePos = namePos + vec2(70, 0);
	char timeBuf[32];
	sprintf(timeBuf, "%.2f", *GetDouble(self->GetData()));
	self->changed = true;
	ImDrawText(timePos, timeBuf, vec3(0.3, 0, 0));
}