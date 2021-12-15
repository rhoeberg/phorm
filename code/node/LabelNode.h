#pragma once

void LabelOperation(Node *self)
{
	// NEXT
	// Update labels hashmap with new key if name parameter changed

	NodeInput *input = &self->inputs[0];
	if(!input || !input->handleIsset) {
		return;
		NOT_IMPLEMENTED
	}

	String *string = GetString(self->params[0].dataHandle);
	if(!string) {
		return;
		NOT_IMPLEMENTED
	}

	// delete old key from hashmap
	LabelNodeState *state = &_nodeState->labelNodes[self->extraHandle];
	_nodeState->labels.Remove(state->key);

	// add new key to hashmap
	_nodeState->labels.Insert(*string, input->handle);
	state->key = *string;
}

void DrawLabelNode(Node *self)
{
	self->rect.width = 200;
	self->rect.height = 30;

	ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
	ImDrawRect(self->rect);

	vec2 namePos = self->rect.pos + vec2(10.0f, self->rect.height - 10.0f);
	ImDrawText(namePos, self->name);

	vec2 labelPos = namePos + vec2(70, 0);
	String *str = GetString(self->params[0].dataHandle);
	ImDrawText(labelPos, str->buffer, vec3(0.3, 0, 0));
}

ObjectHandle SetupLabelNode()
{
	return _nodeState->labelNodes.Insert(LabelNodeState());
}
