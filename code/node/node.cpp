#include "node.h"

DataHandle Node::GetData()
{
	if(Changed()) {
		changed = false;
		op(this);
	}

	return dataHandle;
}

bool Node::Changed()
{
	bool inputChanged = false;
	for(int i = 0; i < inputs.Count(); i++) {
		if(inputs[i].handleIsset) {
			Node *inputNode = GetNode(inputs[i].handle);
			if(inputNode->Changed())
				inputChanged = true;
		}
	}

	for(int i = 0; i < params.Count(); i++) {
		if(params[i].handleIsset) {
			Node *paramNode = GetNode(params[i].handle);
			if(paramNode->changed) {
				inputChanged = true;
			}
		}
	}

	if(inputChanged || changed) {
		return true;
	}

	return false;
}

double NodeParameter::Double()
{
	if(handleIsset) {
		return *GetDoubleOutput(handle);
	}

	if(type == DATA_DOUBLE)
		return d;

	return 0.0;
}

void BaseNodeDrawFunction(Node *node)
{
	// Rect rect = GetNodeRect(handle);
	node->rect.width = NODE_BASE_WIDTH;
	node->rect.width += node->params.Count() * PARAM_WIDTH;
	node->rect.width += node->inputs.Count() * PARAM_WIDTH;
	node->rect.height = NODE_HEIGHT;
	// Rect rect = node->rect;
	ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
	ImDrawRect(node->rect);

	vec2 namePos = node->rect.pos + vec2(10.0f, node->rect.height - 10.0f);
	ImDrawText(namePos, node->name);
}

int AddNode(const char *name, NodeDataType type, NodeOperation op, VMArray<NodeParameter> params, VMArray<NodeInput> inputs)
{
	return AddNode(name, type, op, BaseNodeDrawFunction, params, inputs);
}

int AddNode(const char *name, NodeDataType type, NodeOperation op, NodeDrawingFunction drawingFunction, VMArray<NodeParameter> params, VMArray<NodeInput> inputs)
{
	// TODO (rhoe) since similar node type dont change name we could
	//             store the names in a string array and just refer to the handle
	Node node = {};
	sprintf(node.name, "%s", name);
	int winWidth, winHeight;
	GetWindowSize(&winWidth, &winHeight);
	node.rect.pos = vec2(winWidth / 2, winHeight / 2);
	// node.rect.w = NODE_BASE_WIDTH;
	// node.rect.h = NODE_HEIGHT;
	node.changed = true;
	// node.changeVer = 1;
	// node.lastChangeVer = 0;
	node.inputs = VMArray<NodeInput>();
	node.type = type;
	node.params = params;
	node.inputs = inputs;
	node.op = op;
	node.drawingFunction = drawingFunction;
	DataHandle dataHandle = {};
	dataHandle.type = type;

	switch(node.type) {
		case DATA_TEXTURE: {
			dataHandle.id = _nodeState->textures.InsertNew();
			break;
		}
		case DATA_MESH: {
			Mesh mesh = {};
			dataHandle.id = _nodeState->meshes.Insert(mesh);
			break;
		}
		case DATA_RENDEROBJECT: {
			RenderObject renderObject = {};
			dataHandle = AddNewRenderObject();
			break;
		}
		case DATA_DOUBLE: {
			dataHandle.id = _nodeState->doubles.Insert(0.0);
			break;
		}
	}

	node.SetDataHandle(dataHandle);
	return _nodeState->nodes.Insert(node);
}
