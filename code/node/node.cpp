#include "node.h"

int GetPixelIndex(int x, int y)
{
	return ((y * TEXTURE_SIZE) + x);
}

bool NodeExists(int handle)
{
	if(handle > _nodeState->nodes.Count() - 1  ||
	   handle < 0) {
		return false;
	}

	return true;
}

Node* GetNode(int handle)
{
	if(!NodeExists(handle)) {
		return NULL;
	}

	return &_nodeState->nodes[handle];
}

Texture* GetTexture(int handle)
{
	return &_nodeState->textures[handle];
}

Texture* GetTextureInput(NodeInput input)
{
	if(input.type != TEXTURE_NODE ||
	   input.handle > _nodeState->textures.Count() - 1 ||
	   input.handle < 0) {
		return NULL;
	}

	Node *inputNode = GetNode(input.handle);
	Texture *texture = GetTexture(inputNode->GetData());
	return texture;
}

RenderObject* GetRenderObject(int handle)
{
	return &_nodeState->renderObjects[handle];
}

bool ConnectNodes(int inHandle, int outHandle, int inputIndex)
{
	if(inHandle == outHandle ||
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
	_nodeState->renderObjects = VMArray<RenderObject>();

	// TESTING STUFF
	int loadHandle = AddLoadTextureNode();
	int blurHandle = AddBlurNode();
	int mixNodeHandle = AddMixTextureNode();
	ConnectNodes(mixNodeHandle, loadHandle, 0);
	ConnectNodes(mixNodeHandle, blurHandle, 1);
	ConnectNodes(blurHandle, loadHandle, 0);
}

// TODO (rhoe) change this to node constructor?
int AddNode(const char *name, NodeType type, NodeOperation op, VMArray<NodeParameter> params, VMArray<NodeInput> inputs)
{
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

	switch(node.type) {
		case TEXTURE_NODE:
			node.SetDataHandle(_nodeState->textures.InsertNew());
			break;
		case RENDEROBJECT_NODE:
			RenderObject renderObject = {};
			glGenVertexArrays(1, &renderObject.VAO);
			glGenTextures(1, &renderObject.textureID);
			int handle = _nodeState->renderObjects.Insert(renderObject);
			node.SetDataHandle(handle);
			break;
	}

	return _nodeState->nodes.Insert(node);
}

void UpdateNodes()
{
}

void CleanupNodes()
{
	_nodeState->nodes.Free();
	_nodeState->textures.Free();
	free(_nodeState);
}


