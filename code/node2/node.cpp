#include "node.h"

int GetPixelIndex(int x, int y)
{
	return ((y * TEXTURE_SIZE) + x);
}

void ConnectNodes(int inputHandle, int outputHandle, int inputIndex)
{
	if(inputHandle > -1 && inputHandle < _nodeState->nodes.Count() - 1) {
		if(outputHandle > -1 && outputHandle < _nodeState->nodes.Count() - 1) {
			if(inputHandle != outputHandle) {
				Node *inputNode = &_nodeState->nodes[inputHandle];
				inputNode->ConnectInput(outputHandle, inputIndex);
			}
		}
	}
}

void InitializeNodes()
{
	_nodeState = (NodeState*)malloc(sizeof(NodeState));

	// _nodeState->textureNodes = VMArray<TextureNode>();
	_nodeState->nodes = VMArray<Node>();
	_nodeState->blurNodes = VMArray<BlurNode>();
	_nodeState->loadTextureNodes = VMArray<LoadTextureNode>();
	_nodeState->textures = VMArray<Texture>();

	// TESTING STUFF
	int loadHandle = AddLoadTextureNode();
	_nodeState->blurNodeHandle = AddBlurNode();
	Node *blurNode = &_nodeState->nodes[_nodeState->blurNodeHandle];

	blurNode->ConnectInput(loadHandle, 0);
}

int AddNode()
{
	Node node = {};
	node.rect.pos = vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	node.rect.width = 100;
	node.rect.height = 40;
	node.changed = true;
	node.inputs = VMArray<NodeInput>();

	return _nodeState->nodes.Insert(node);
}

int RunNodeOperation(Node *node)
{
	if(node->changed) {
		node->op(node);
		node->changed = false;
	}

	return node->dataHandle;
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
	_nodeState->blurNodes.Free();
	_nodeState->loadTextureNodes.Free();
	free(_nodeState);
}


