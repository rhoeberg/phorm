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
	Node *inputNode = GetNode(input.handle);

	if(inputNode == NULL ||
	   input.type != TEXTURE_NODE ||
	   inputNode->GetDataLast() > _nodeState->textures.Count() - 1 ||
	   inputNode->GetDataLast() < 0) {
		return NULL;
	}

	Texture *texture = GetTexture(inputNode->GetData());
	return texture;
}

Mesh* GetMesh(int handle)
{
	return &_nodeState->meshes[handle];
}

Mesh* GetMeshInput(NodeInput input)
{
	Node *inputNode = GetNode(input.handle);

	if(inputNode == NULL ||
	   input.type != MESH_NODE ||
	   inputNode->GetDataLast() > _nodeState->meshes.Count() - 1 ||
	   inputNode->GetDataLast() < 0) {
		return NULL;
	}
	   
	return GetMesh(inputNode->GetData());
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

int AddNewRenderObject()
{
	RenderObject renderObject = {};
	glGenVertexArrays(1, &renderObject.VAO);
	glGenTextures(1, &renderObject.textureID);
	int handle = _nodeState->renderObjects.Insert(renderObject);
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

	switch(node.type) {
		case TEXTURE_NODE: {
			node.SetDataHandle(_nodeState->textures.InsertNew());
			break;
		}
		case MESH_NODE: {
			Mesh mesh = {};
			node.SetDataHandle(_nodeState->meshes.Insert(mesh));
			break;
		}
		case RENDEROBJECT_NODE: {
			RenderObject renderObject = {};
			glfwMakeContextCurrent(_viewerWindow);
			glGenVertexArrays(1, &renderObject.VAO);
			glGenBuffers(1, &renderObject.EBO);
			glGenBuffers(1, &renderObject.VBO);
			glGenTextures(1, &renderObject.textureID);
			glfwMakeContextCurrent(_win);
			int handle = _nodeState->renderObjects.Insert(renderObject);
			node.SetDataHandle(handle);
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


