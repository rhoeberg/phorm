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
	if(input.type != TEXTURE_NODE) {
		return NULL;
	}

	Node *inputNode = GetNode(input.handle);
	Texture *texture = GetTexture(inputNode->GetData());
	return texture;
}

void ConnectNodes(int inHandle, int outHandle, int inputIndex)
{
	if(inHandle == outHandle ||
	   !NodeExists(inHandle) ||
	   !NodeExists(outHandle)) {
		return;
	}

	Node *inputNode = GetNode(inHandle);
	if(inputNode != NULL) {
		inputNode->inputs[inputIndex].handle = outHandle;
	}
}

void InitializeNodes()
{
	_nodeState = (NodeState*)malloc(sizeof(NodeState));

	// _nodeState->textureNodes = VMArray<TextureNode>();
	_nodeState->nodes = VMArray<Node>();

	// data containers
	_nodeState->textures = VMArray<Texture>();

	// TESTING STUFF
	int loadHandle = AddLoadTextureNode();
	int blurHandle = AddBlurNode();
	// Node *blurNode = &_nodeState->nodes[_nodeState->blurNodeHandle];

	int mixNodeHandle = AddMixTextureNode();
	// Node *mixNode = &_nodeState->nodes[mixNodeHandle];

	ConnectNodes(mixNodeHandle, loadHandle, 0);
	ConnectNodes(mixNodeHandle, blurHandle, 1);

	ConnectNodes(blurHandle, loadHandle, 0);
}

// TODO (rhoe) change this to node constructor?
int AddNode(const char *name, NodeType type, NodeOperation op, VMArray<NodeParameter> params, VMArray<NodeInput> inputs)
{
	Node node = {};
	sprintf(node.name, "%s", name);
	node.pos = vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	// node.rect.width = 150;
	// node.rect.height = 40;
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
	}

	return _nodeState->nodes.Insert(node);
}

void UpdateNodes()
{
	// ///////////////////
	// // TESTING STUFF
	// Node *node = &_nodeState->nodes[_nodeState->blurNodeHandle];
	// // Node *inputNode = &_nodeState->nodes[node->inputs[0].handle];

	// int dataHandle = RunNodeOperation(node);
	// Texture *texture = &_nodeState->textures[dataHandle];
	// // Pixel *pixels = node->GetPixels(node);

	// glBindTexture(GL_TEXTURE_2D, _nodeState->textureID);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
	// glBindTexture(GL_TEXTURE_2D, 0);

	// // render texture to quad here
	// glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_2D, _nodeState->textureID);
	// glUseProgram(GetTextureShader());

	// glBindVertexArray(_nodeState->viewerQuad);
	// glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	// GLuint modelLoc = glGetUniformLocation(GetTextureShader(), "model");
	// glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	// glBindVertexArray(0);
}

void CleanupNodes()
{
	_nodeState->nodes.Free();
	_nodeState->textures.Free();
	free(_nodeState);
}


