#include "node.h"

int GetPixelIndex(int x, int y)
{
	return ((y * TEXTURE_SIZE) + x);
}

bool NodeExists(NodeHandle handle)
{
	if(handle.id > _nodeState->nodes.Count() - 1  ||
	   handle.type == NIL_NODE) {
		return false;
	}

	return true;
}

Node* GetNode(NodeHandle handle)
{
	if(!NodeExists(handle)) {
		return NULL;
	}

	return &_nodeState->nodes[handle.id];
}

Texture* GetTexture(DataHandle handle)
{
	if(handle.type == TEXTURE_NODE)
		return &_nodeState->textures[handle.id];
	else
		return NULL;
}

Texture* GetTextureInput(NodeInput input)
{
	Node *inputNode = GetNode(input.handle);

	if(inputNode == NULL ||
	   input.type != TEXTURE_NODE ||
	   inputNode->GetDataLast().id > _nodeState->textures.Count() - 1 ||
	   inputNode->GetDataLast().id < 0) {
		return NULL;
	}

	Texture *texture = GetTexture(inputNode->GetData());
	return texture;
}

Mesh* GetMesh(DataHandle handle)
{
	if(handle.type != MESH_NODE) {
		// TODO err
		return NULL;
	}
	
	return &_nodeState->meshes[handle.id];
}

Mesh* GetMeshInput(NodeInput input)
{
	Node *inputNode = GetNode(input.handle);

	if(inputNode == NULL ||
	   input.type != MESH_NODE ||
	   inputNode->GetDataLast().id > _nodeState->meshes.Count() - 1 ||
	   inputNode->GetDataLast().id < 0) {
		return NULL;
	}
	   
	return GetMesh(inputNode->GetData());
}

RenderObject* GetRenderObject(DataHandle handle)
{
	if(handle.type != RENDEROBJECT_NODE) {
		// TODO err
		return NULL;
	}
	
	return &_nodeState->renderObjects[handle.id];
}

bool ConnectNodes(NodeHandle inHandle, NodeHandle outHandle, int inputIndex)
{
	if(inHandle.id == outHandle.id ||
	   !NodeExists(inHandle) ||
	   !NodeExists(outHandle)) {
		return false;
	}

	Node *inputNode = GetNode(inHandle);
	if(inputNode != NULL) {
		inputNode->changed = true;
		inputNode->inputs[inputIndex].handle = outHandle;
	}

	return true;
}

void InitializeNodes()
{
	_nodeState = (NodeState*)malloc(sizeof(NodeState));

	_nodeState->nodes = VMArray<Node>();

	// data containers
	_nodeState->textures = VMArray<Texture>();
	_nodeState->meshes = VMArray<Mesh>();
	_nodeState->renderObjects = VMArray<RenderObject>();

	// TESTING STUFF
	// int loadHandle = AddLoadTextureNode();
	// int blurHandle = AddBlurNode();
	// int mixNodeHandle = AddMixTextureNode();
	// ConnectNodes(mixNodeHandle, loadHandle, 0);
	// ConnectNodes(mixNodeHandle, blurHandle, 1);
	// ConnectNodes(blurHandle, loadHandle, 0);
}

DataHandle AddNewRenderObject()
{
	RenderObject renderObject = {};

	renderObject.VAOHandle = AddVAO();
	glGenBuffers(1, &renderObject.EBO);
	glGenBuffers(1, &renderObject.VBO);
	glGenTextures(1, &renderObject.textureID);

	glGenTextures(1, &renderObject.textureID);

	DataHandle handle = {};
	handle.type = RENDEROBJECT_NODE;
	handle.id = _nodeState->renderObjects.Insert(renderObject);
	return handle;
}

// TODO (rhoe) change this to node constructor?
int AddNode(const char *name, NodeType type, NodeOperation op, VMArray<NodeParameter> params, VMArray<NodeInput> inputs)
{
	// TODO (rhoe) since similar node type dont change name we could
	//             store the names in a string array and just refer to the handle
	Node node = {};
	sprintf(node.name, "%s", name);
	int winWidth, winHeight;
	GetWindowSize(&winWidth, &winHeight);
	node.pos = vec2(winWidth / 2, winHeight / 2);
	node.changed = true;
	node.inputs = VMArray<NodeInput>();
	node.type = type;
	node.params = params;
	node.inputs = inputs;
	node.op = op;
	DataHandle dataHandle = {};
	dataHandle.type = type;

	switch(node.type) {
		case TEXTURE_NODE: {
			dataHandle.id = _nodeState->textures.InsertNew();
			node.SetDataHandle(dataHandle);
			break;
		}
		case MESH_NODE: {
			Mesh mesh = {};
			dataHandle.id = _nodeState->meshes.Insert(mesh);
			node.SetDataHandle(dataHandle);
			break;
		}
		case RENDEROBJECT_NODE: {
			RenderObject renderObject = {};
			dataHandle = AddNewRenderObject();
			node.SetDataHandle(dataHandle);
			break;
		}
	}

	return _nodeState->nodes.Insert(node);
}

void UpdateNodes()
{
}

void CleanupNodes()
{
	// TODO (rhoe) freeing moar stuff
	// we should also free all the arrays in the node
	// like input and param
	_nodeState->nodes.Free();
	_nodeState->textures.Free();

	
	free(_nodeState);
}


