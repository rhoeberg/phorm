#include "node.h"

ObjectHandle Node::GetData()
{
	if(Changed()) {
		changed = false;
		CallOp();
	}

	return dataHandle;
}

void Node::CallOp()
{
	op(this);
}

void Node::CallDraw()
{
	drawFunc(this);
}

bool Node::Changed()
{
	bool inputChanged = false;
	for(int i = 0; i < inputs.Count(); i++) {
		if(inputs[i].handle.isset) {
			Node *inputNode = GetNode(inputs[i].handle);
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
			Node *paramNode = GetNode(params[i].nodeHandle);
			if(!paramNode) {
				inputChanged = true;
			}
			else {
				if(paramNode->Changed()) {
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

ObjectHandle AddNode(const char *name, DataType type, NodeOp op, NodeDrawFunc drawFunc, FixedArray<NodeParameter> params, FixedArray<NodeInput> inputs, ObjectHandle extraHandle)
{
	Node node = {};
	sprintf(node.name, "%s", name);
	node.rect.pos = GetNextNewNodePos();
	node.changed = true;
	node.type = type;
	node.params = params;
	node.inputs = inputs;
	node.op = op;
	node.drawFunc = drawFunc;
	node.extraHandle = extraHandle;
	node.labelHandle = AddString(name);
	ObjectHandle dataHandle = {};

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
		case DATA_RENDEROBJECT_GROUP: {
			dataHandle = _nodeState->renderObjectGroups.Insert(RenderObjectGroup());
			break;
		}
		case DATA_POINTLIGHT: {
			PointLight light = {};
			dataHandle = _nodeState->pointLights.Insert(light);
			break;
		}
		case DATA_DOUBLE: {
			double value = 0.0;
			dataHandle = _nodeState->doubles.Insert(value);
			break;
		}
		case DATA_INT: {
			int value = 0;
			dataHandle = GetInts()->Insert(value);
			break;
		}
		case DATA_VEC3: {
			vec3 v3 = {};
			dataHandle = _nodeState->vec3s.Insert(v3);
			break;
		}
		case DATA_NONE: {
			break;
		}
		default: {
			NOT_IMPLEMENTED;
		}
	}

	node.SetDataHandle(dataHandle);
	return _nodeState->nodes.Insert(node);
}

bool ConnectNodeParameter(ObjectHandle handle, ObjectHandle outHandle, int paramIndex)
{
	Node *node = GetNode(handle);
	Node *outputNode = GetNode(outHandle);

	if(!node || !outputNode) {
		return false;
	}

	if(handle.id == outHandle.id ||
	   node->params[paramIndex].type != outputNode->type) {
		return false;
	}

	if(node != NULL) {
		node->changed = true;
		node->params[paramIndex].nodeHandle = outHandle;
	}

	return true;
}

bool ConnectNodeInput(ObjectHandle handle, ObjectHandle outHandle, int inputIndex)
{
	Node *inputNode = GetNode(handle);
	Node *outputNode = GetNode(outHandle);

	if(!inputNode || !outputNode) {
		return false;
	}

	if(handle.id == outHandle.id ||
	   inputNode->inputs[inputIndex].type != outputNode->type) {
		return false;
	}

	if(inputNode != NULL) {
		inputNode->changed = true;
		inputNode->inputs[inputIndex].handle = outHandle;
	}

	return true;
}

bool NodeExists(ObjectHandle handle)
{
	return _nodeState->nodes.Exists(handle);
}

Node* GetNode(ObjectHandle handle)
{
	if(!NodeExists(handle)) {
		return NULL;
	}

	return _nodeState->nodes.Get(handle);
}

void DeleteNode(ObjectHandle handle)
{
	_nodeState->nodes.Remove(handle);
}

void CleanupNodes()
{
	// TODO (rhoe) freeing moar stuff
	// we should also free all the arrays in the node
	// like input and param
	_nodeState->nodes.Free();
	_nodeState->textures.Free();

	for(int i = 0; i < _nodeState->strings.Count(); i++) {
		ObjectHandle handle = _nodeState->strings.GetHandle(i);
		String *string = _nodeState->strings.Get(handle);
		string->Free();
	}
	_nodeState->strings.Free();

	free(_nodeState);
}
