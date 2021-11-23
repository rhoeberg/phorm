#include "node.h"

int GetPixelIndex(int x, int y)
{
	return ((y * TEXTURE_SIZE) + x);
}

void InitializeNodes()
{
	_nodeState = (NodeState*)malloc(sizeof(NodeState));

	_nodeState->textureNodes = VMArray<TextureNode>();
	_nodeState->blurNodes = VMArray<BlurNode>();
	_nodeState->loadTextureNodes = VMArray<LoadTextureNode>();


	// initialize drag and hover state
	_nodeState->isDragging = false;
	_nodeState->hoverState.hoveringElement = false;

	// TESTING STUFF
	int loadHandle = AddLoadTextureNode();
	_nodeState->blurNodeHandle = AddBlurNode();
	TextureNode *blurNode = GetTextureNode(_nodeState->blurNodeHandle);
	blurNode->inputs[0].type = TEXTURE_NODE;
	blurNode->inputs[0].handle = loadHandle;
	// BlurNode *blu = GetTextureNode(loadHandle);


	glGenTextures(1, &_nodeState->textureID);
	glBindTexture(GL_TEXTURE_2D, _nodeState->textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	_nodeState->viewerQuad = createSpriteVAO();
}

void DrawNode(int handle)
{
	TextureNode *node = GetTextureNode(handle);
	Rect rect = {};
	ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
	ImDrawRect(node->rect);
}

void UpdateHoverState()
{
	for(int i = 0; i < _nodeState->textureNodes.Count(); i++) {
		if(PointInsideRect(mouse, _nodeState->textureNodes[i].rect)) {
			_nodeState->hoverState.nodeHandle = i;
			_nodeState->hoverState.hoveringElement = true;
		}
	}
}

void UpdateNodeDragging()
{
	if(_nodeState->hoverState.hoveringElement) {
		if(mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] && !_nodeState->isDragging) {
			_nodeState->draggedNodeHandle = _nodeState->hoverState.nodeHandle;
			_nodeState->isDragging = true;
		}
	}

	if(_nodeState->isDragging) {

		if(!mouse_buttons[GLFW_MOUSE_BUTTON_LEFT]) {
			// handle stopped dragging
			_nodeState->isDragging = false;
		}
		else {
			// handle continued dragging
			_nodeState->textureNodes[_nodeState->draggedNodeHandle].rect.pos = mouse;
		}
	}
}

void UpdateNodes()
{

	//////////////////
	// CHECK HOVERSTATE
	UpdateHoverState();

	UpdateNodeDragging();

	//////////////////
	// DRAW NODES
	for(int i = 0; i < _nodeState->textureNodes.Count(); i++) {
		DrawNode(i);
	}

	///////////////////
	// TESTING STUFF
	TextureNode *node = &_nodeState->textureNodes[_nodeState->blurNodeHandle];
	Pixel *pixels = node->GetPixels(node);

	glBindTexture(GL_TEXTURE_2D, _nodeState->textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	// render texture to quad here
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _nodeState->textureID);
	glUseProgram(GetTextureShader());

	glBindVertexArray(_nodeState->viewerQuad);
	glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	GLuint modelLoc = glGetUniformLocation(GetTextureShader(), "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void NodeGUI()
{
	ImGui::Begin("Inspector");
	if(ImGui::Button("add blur node")) {
		AddBlurNode();
	}
	ImGui::End();
}

void CleanupNodes()
{
	free(_nodeState);
}


