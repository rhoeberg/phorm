#include "node.h"

ObjectHandle Node::GetData()
{
	if(Changed()) {
		changed = false;
		// op(this);
		CallNodeOp(this);
	}

	return dataHandle;
}

void Node::CallOp()
{
	CallNodeOp(this);
}

bool Node::Changed()
{
	bool inputChanged = false;
	for(int i = 0; i < inputs.Count(); i++) {
		if(inputs[i].handle.isset) {
			Node *inputNode = GetNode(&inputs[i].handle);
			if(!inputNode) {
				inputChanged = true;
			}
			else {
				if(inputNode->Changed())
					inputChanged = true;
			}
		}
	}

	for(int i = 0; i < params.Count(); i++) {
		if(params[i].nodeHandle.isset) {
			Node *paramNode = GetNode(&params[i].nodeHandle);
			if(!paramNode) {
				inputChanged = true;
			}
			else {
				if(paramNode->changed) {
					inputChanged = true;
				}
			}
		}
	}

	if(inputChanged || changed) {
		return true;
	}

	return false;
}

ObjectHandle AddNode(const char *name, DataType type, NodeOp op, FixedArray<NodeParameter> params, FixedArray<NodeInput> inputs, ObjectHandle extraHandle)
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
	node.type = type;
	node.params = params;
	node.inputs = inputs;
	node.op = op;
	node.extraHandle = extraHandle;
	// node.drawFunc = drawFunc;
	ObjectHandle dataHandle = {};
	// dataHandle.type = type;

	switch(node.type) {
		case DATA_TEXTURE: {
			dataHandle = _nodeState->textures.InsertNew();
			break;
		}
		case DATA_MESH: {
			Mesh mesh = {};
			dataHandle = _nodeState->meshes.Insert(mesh);
			break;
		}
		case DATA_RENDEROBJECT: {
			dataHandle = AddNewRenderObject();
			break;
		}
		case DATA_DOUBLE: {
			double value = 0.0;
			dataHandle = _nodeState->doubles.Insert(value);
			break;
		}
	}

	node.SetDataHandle(dataHandle);
	return _nodeState->nodes.Insert(node);
}
