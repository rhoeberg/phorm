#include "node.h"

void Node::AddInput(DataType type)
{
	NodeInput input = {};
	input.type = type;
	input.handle = {};
	inputs.Insert(input);
}

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

ObjectHandle Node::Copy()
{
	NodeConstructor *nodeConstructor = GetNodeConstructors()->Get(name);
	ObjectHandle resultHandle = ConstructNode(String(name), nodeConstructor);
	Node *resultNode = GetNode(resultHandle);

	resultNode->rect = rect;
	resultNode->rect.pos += vec2(5, 5);
	resultNode->inputs = inputs;
	resultNode->params = params;

	return resultHandle;
}

ObjectHandle AddNode(DataType type, FixedArray<NodeParameter> params, FixedArray<NodeInput> inputs)
{
	Node node = {};
	node.rect.pos = GetNextNewNodePos();
	node.changed = true;
	node.type = type;
	node.params = params;
	node.inputs = inputs;
	node.page = GetCurrentPage();
	return _nodeState->nodes.Insert(node);
}

// TODO (rhoe) perhaps we should allow auto casting of int and doubles
// bool ValidInputType(DataType a, DataType b)
// {
// 	if(a == b)
// 		return true;

// 	if(a == DATA_DOUBLE && b == DATA_INT ||
// 	   a == DATA_INT && b == DATA_DOUBLE)
// 		return true;

// 	return false;
// }

bool ConnectNodeParameter(ObjectHandle handle, ObjectHandle outHandle, int paramIndex)
{
	Node *node = GetNode(handle);
	Node *outputNode = GetNode(outHandle);

	if(!node || !outputNode) {
		return false;
	}

	if(handle.id == outHandle.id ||
	   node->params[paramIndex].type != outputNode->type) {
	   // !ValidInputType(node->params[paramIndex].type, outputNode->type)) {
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

ObjectHandle GetInputData(NodeInput input)
{
	Node *node = GetNode(input.handle);
	if(node) {
		return node->GetData();
	}

	return ObjectHandle();
}

void CleanupNodes()
{
	// TODO (rhoe) freeing moar stuff
	// we should also free all the arrays in the node
	// like input and param
	_nodeState->nodes.Free();
	_nodeState->bitmaps.Free();

	for(int i = 0; i < _nodeState->strings.Count(); i++) {
		ObjectHandle handle = _nodeState->strings.GetHandle(i);
		String *string = _nodeState->strings.Get(handle);
		string->Free();
	}
	_nodeState->strings.Free();

	free(_nodeState);
}
