#pragma once

struct NodeInput {
	DataType type;
	ObjectHandle handle;

	NodeInput(DataType _type) {
		type = _type;
		handle.isset = false;
	}

	NodeInput() {}
};

struct Node;
typedef void (*NodeOp)(Node *self);
typedef void (*NodeDrawFunc)(Node *self);
typedef void (*NodeCustomEditor)(Node *self);

struct Node {
	NodeOp op;
	NodeDrawFunc drawFunc;
	NodeCustomEditor editor;
	DataType type; //defines the return of the node operation
	Rect rect;
	bool changed;
	char name[128];
	FixedArray<NodeInput> inputs;
	FixedArray<NodeParameter> params;
	ObjectHandle extraHandle;
	ObjectHandle labelHandle;
	bool initialized;

	void AddInput(DataType type)
	{
		NodeInput input = {};
		input.type = type;
		input.handle = {};
		inputs.Insert(input);
	}

	void SetDataHandle(ObjectHandle handle) { dataHandle = handle; }

	ObjectHandle GetDataLast() { return dataHandle; }
	ObjectHandle GetData();
	void CallOp();
	void CallDraw();

	bool Changed();

private:
	ObjectHandle dataHandle;
};

ObjectHandle AddNode(const char *name, DataType type, NodeOp op, NodeDrawFunc drawFunc, FixedArray<NodeParameter> params, FixedArray<NodeInput> inputs, ObjectHandle extraHandle = ObjectHandle());

bool NodeExists(ObjectHandle handle);
Node* GetNode(ObjectHandle handle);
void DeleteNode(ObjectHandle handle);

bool ConnectNodeParameter(ObjectHandle handle, ObjectHandle outHandle, int paramIndex);
bool ConnectNodeInput(ObjectHandle inHandle, ObjectHandle outHandle, int inputIndex);
