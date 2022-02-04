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

	// RUNTIME SETUP
	NodeOp op;
	NodeDrawFunc drawFunc;
	NodeCustomEditor editor;
	bool changed;
	bool initialized;

	// PERSISTANT
	// TODO (rhoe) perhaps this should go in seperate struct/array
	DataType type; // data type of output
	Rect rect;
	i32 page;
	char name[128];
	ObjectHandle extraHandle;
	ObjectHandle labelHandle;
	FixedArray<NodeInput> inputs;
	FixedArray<NodeParameter> params;

	void AddInput(DataType type);
	void SetDataHandle(ObjectHandle handle) { dataHandle = handle; }
	ObjectHandle GetDataLast() { return dataHandle; }
	ObjectHandle GetData();
	void CallOp();
	void CallDraw();
	bool Changed();

private:
	ObjectHandle dataHandle;
};

ObjectHandle AddNode(DataType type, FixedArray<NodeParameter> params, FixedArray<NodeInput> inputs);

bool NodeExists(ObjectHandle handle);
Node* GetNode(ObjectHandle handle);
void DeleteNode(ObjectHandle handle);
ObjectHandle GetInputData(NodeInput input);

bool ConnectNodeParameter(ObjectHandle handle, ObjectHandle outHandle, int paramIndex);
bool ConnectNodeInput(ObjectHandle inHandle, ObjectHandle outHandle, int inputIndex);
