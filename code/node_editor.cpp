/*

  to link nodes maybe we can use a kind of buffer that we write to and read from
  so a node writes its outputs to its output buffer
  when another node is linked to this and needs its output for one of its inputs
  you simply lookup the output buffer of the node to get the result.

  how will this work with execution order?
  the old result can linger which mean sometimes a node gets an old result hmm.

  or can we do something smarter?
  maybe we can keep track of which nodes are updated in the frame,
  when we then need an input which has not been updated we first update it to get the latest
  output.

  

  MODULE IDEAS:
  - time 
  - sin/cos

 */


#include "node_editor.h"
#include <math.h>

void NodeEditorInitialize()
{
	_nodeState = (NodeState*)malloc(sizeof(NodeState));
	_nodeState->nodes = (Node*)malloc(sizeof(Node) * MAX_NODE_AMOUNT);
	_nodeState->nodeAmount = 0;
	_nodeState->isDragging = false;
}

void CubeFunction(Node *self)
{
	// get input
	vec3 pos = vec3(0, 0, 0);
	if(self->inputs[0].attached) {
		Node *attachedNode = GetNode(self->inputs[0].nodeHandle);
		if(attachedNode != NULL) {
			int outputIndex = self->inputs[0].nodeOutputIndex;
			pos = attachedNode->outputs[outputIndex].data.v3;
		}
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ftex);
	// glUseProgram(textShader);

	// render cube
	glUseProgram(shaderProgram);
	glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	model = glm::translate(model, pos);
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0, 1, 0));
	GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glUseProgram(0);
}

void NoneFunction(Node *self)
{
	self->outputs[0].data.v3.x = sin(glfwGetTime());
}

// TODO (rhoe) we need some way to avoid problems with dangling handles
//             if old node slots are reused
int AddNode()
{
	int result = -1;

	// first check if there is a free slot
	bool foundFree = false;
	for(int i = 0; i < _nodeState->nodeAmount; i++) {
		if(_nodeState->nodes[i].free) {
			result = i;
			foundFree = true;
		}
	}

	// try to add one to the end
	if(!foundFree) {
		if(_nodeState->nodeAmount < MAX_NODE_AMOUNT) {
			result = _nodeState->nodeAmount;
			_nodeState->nodeAmount++;
		}
	}

	// we found a slot now initialize it
	if(result != -1) {
		Node *node = &_nodeState->nodes[result];
		node->free = false;
		node->pos = vec2(0, 0);
		// node->rect.width = 0.1f;
		// node->rect.height = 0.1f;

		for(int i = 0; i < ARRAY_SIZE(node->inputs); i++) {
			node->inputs[i].active = false;
			node->inputs[i].attached = false;
		}
		for(int i = 0; i < ARRAY_SIZE(node->outputs); i++) {
			node->outputs[i].active = false;
		}

		sprintf(node->name, "empty node");
	}

	

	return result;
}

Node* GetNode(int handle)
{
	if(handle > MAX_NODE_AMOUNT - 1) {
		return NULL;
	}

	return &_nodeState->nodes[handle];
}

int GetNodeInputSize(int handle)
{
	int result = 0;

	Node *node = GetNode(handle);
	for(int i = 0; i < ARRAY_SIZE(node->inputs); i++) {
		if(!node->inputs[i].active)
			break;

		result++;
	}

	return result;
}

int GetNodeOutputSize(int handle)
{
	int result = 0;

	Node *node = GetNode(handle);
	for(int i = 0; i < ARRAY_SIZE(node->outputs); i++) {
		if(!node->outputs[i].active)
			break;

		result++;
	}

	return result;
}

void AttachNodeSockets(int inputNodeHandle, int inputIndex,
					   int outputNodeHandle, int outputIndex)
{
	Node *inputNode = GetNode(inputNodeHandle);
	Node *outputNode = GetNode(outputNodeHandle);
	NodeInput *input = &inputNode->inputs[inputIndex];
	NodeOutput *output = &outputNode->outputs[outputIndex];

	if(input->type == output->type) {
		input->nodeHandle = outputNodeHandle;
		input->nodeOutputIndex = outputIndex;
		input->attached = true;
	}
	else {
		// handle error when not able to attach nodes
	}
}

void DeattachNodeSockets(int inputNodeHandle, int inputIndex)
{
	Node *inputNode = GetNode(inputNodeHandle);
	NodeInput *input = &inputNode->inputs[inputIndex];
	input->attached = false;
}

Rect GetNodeRect(int handle)
{
	Rect result = {};

	Node *node = GetNode(handle);
	result.pos = node->pos;

	int inputSize = GetNodeInputSize(handle);
	int outputSize = GetNodeOutputSize(handle);

	int maxSize = (inputSize > outputSize) ? inputSize : outputSize;

	result.height = 0.1f;
	result.width = 0.1f + (0.1f * maxSize);

	return result;
}

Rect GetNodeInputRect(int nodeHandle, int inputIndex)
{
	Rect result = {};

	// Node *node = GetNode(nodeHandle);
	Rect nodeRect = GetNodeRect(nodeHandle);

	result.pos = nodeRect.pos + vec2(0.1f * inputIndex, nodeRect.height);
	result.width = 0.1f;
	result.height = 0.1f;

	return result;
}

Rect GetNodeOutputRect(int nodeHandle, int inputIndex)
{
	Rect result = {};

	// Node *node = GetNode(nodeHandle);
	Rect nodeRect = GetNodeRect(nodeHandle);

	result.pos = nodeRect.pos + vec2(0.1f * inputIndex, -nodeRect.height);
	result.width = 0.1f;
	result.height = 0.1f;

	return result;
}

int AddCube()
{
	int result = AddNode();
	_nodeState->nodes[result].function = &CubeFunction;

	_nodeState->nodes[result].inputs[0].active = true;
	// _nodeState->nodes[result].inputs[1].active = true;
	_nodeState->nodes[result].outputs[0].active = true;

	sprintf(_nodeState->nodes[result].name, "cube");

	return result;
}

int AddNoneNode()
{
	int result = AddNode();
	_nodeState->nodes[result].function = &NoneFunction;
	_nodeState->nodes[result].outputs[0].active = true;
	return result;
}

bool IsMouseOverNode(vec2 mouse, int handle)
{
	Rect rect = GetNodeRect(handle);
	if(PointInsideRect(mouse, rect))
		return true;

	return false;
}

bool IsMouseOverNodeInput(vec2 mouse, int handle, int ioIndex)
{
	Rect rect = GetNodeInputRect(handle, ioIndex);
	if(PointInsideRect(mouse, rect))
		return true;

	return false;
}

bool IsMouseOverNodeOutput(vec2 mouse, int handle, int ioIndex)
{
	Rect rect = GetNodeOutputRect(handle, ioIndex);
	if(PointInsideRect(mouse, rect))
		return true;

	return false;
}

void DrawNode(int handle)
{
	Rect rect = GetNodeRect(handle);

	Node *node = GetNode(handle);

	// draw inputs
	for(int i = 0; i < GetNodeInputSize(handle); i++) {
		Rect inputRect = GetNodeInputRect(handle, i);
		ImDrawSetColor(vec3(0, 1, 0));
		ImDrawRect(inputRect);

		if(node->inputs[i].attached) {
			Rect outputRect = GetNodeOutputRect(node->inputs[i].nodeHandle, node->inputs[i].nodeOutputIndex);
			ImDrawLine(inputRect.pos, outputRect.pos);
		}
	}

	// draw outputs
	for(int i = 0; i < GetNodeOutputSize(handle); i++) {
		Rect outputRect = GetNodeOutputRect(handle, i);
		ImDrawSetColor(vec3(0, 0, 1));
		ImDrawRect(outputRect);
	}

	ImDrawSetColor(vec3(1, 1, 1));
	ImDrawRect(rect);

	my_stbtt_print(rect.pos.x * SCREEN_WIDTH, rect.pos.y * SCREEN_HEIGHT, node->name);
}

// void TryDraggingNodeInput(int handle, int ioIndex)
// {
// 	if(IsMouseOverNodeInput(mouse, handle, ioIndex)) {
// 		if(mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] && !_nodeState->isDragging) {
// 			_nodeState->isDragging = true;
// 			_nodeState->draggedNodeHandle = handle;
// 			_nodeState->draggedNodeIOIndex = ioIndex;
// 			_nodeState->draggedElementType = EDITOR_ELEMENT_INPUT;
// 		}
// 	}
// }

// void TryDraggingNodeOutput(int handle, int ioIndex)
// {
// 	if(IsMouseOverNodeOutput(mouse, handle, ioIndex)) {
// 		if(mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] && !_nodeState->isDragging) {
// 			_nodeState->isDragging = true;
// 			_nodeState->draggedNodeHandle = handle;
// 			_nodeState->draggedNodeIOIndex = ioIndex;
// 			_nodeState->draggedElementType = EDITOR_ELEMENT_OUTPUT;
// 		}
// 	}
// }

struct HoverState {
	int nodeHandle;
	int ioIndex;
	NodeEditorElementType elementType;
	bool hoveringElement;
};

HoverState GetHoverState()
{
	HoverState result = {};
	for(int i = 0; i < _nodeState->nodeAmount; i++) {
		Node *node = &_nodeState->nodes[i];
		if(IsMouseOverNode(mouse, i)) {
			result.nodeHandle = i;
			result.elementType = EDITOR_ELEMENT_NODE;
			result.hoveringElement = true;
		}
		else {

			// loop over inputs and check mouseover
			for(int j = 0; j < GetNodeInputSize(i); j++) {
				if(IsMouseOverNodeInput(mouse, i, j)) {
					result.nodeHandle = i;
					result.ioIndex = j;
					result.elementType = EDITOR_ELEMENT_INPUT;
					result.hoveringElement = true;
					break;
				}
			}

			// loop over outputs and check mouseover
			if(!result.hoveringElement) {
				for(int j = 0; j < GetNodeOutputSize(i); j++) {
					if(IsMouseOverNodeOutput(mouse, i, j)) {
						result.nodeHandle = i;
						result.ioIndex = j;
						result.elementType = EDITOR_ELEMENT_OUTPUT;
						result.hoveringElement = true;
					}
				}
			}
		}
	}

	return result;
}

void UpdateNodeEditor()
{
	HoverState hoverState = GetHoverState();
	if(hoverState.hoveringElement) {
		if(mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] && !_nodeState->isDragging) {
			_nodeState->draggedNodeHandle = hoverState.nodeHandle;
			_nodeState->draggedNodeIOIndex = hoverState.ioIndex;
			_nodeState->draggedElementType = hoverState.elementType;
			_nodeState->isDragging = true;
			Node *node = GetNode(hoverState.nodeHandle);
			_nodeState->dragOffset = node->pos - mouse;
		}
	}

	// for(int i = 0; i < _nodeState->nodeAmount; i++) {
	// 	Node *node = &_nodeState->nodes[i];
	// 	if(IsMouseOverNode(mouse, i)) {
	// 		if(mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] && !_nodeState->isDragging) {
	// 			_nodeState->isDragging = true;
	// 			_nodeState->draggedNodeHandle = i;
	// 			_nodeState->draggedElementType = EDITOR_ELEMENT_NODE;
	// 			_nodeState->dragOffset = node->pos - mouse;
	// 		}
	// 	}
	// 	else {

	// 		// loop over inputs and check mouseover
	// 		for(int j = 0; j < GetNodeInputSize(i); j++) {
	// 			TryDraggingNodeInput(i, j);
	// 		}

	// 		// loop over outputs and check mouseover
	// 		for(int j = 0; j < GetNodeOutputSize(i); j++) {
	// 			TryDraggingNodeOutput(i, j);
	// 		}
	// 	}

		// DrawNode(i);

		// if(node->in.attached) {
			// draw the line
			// Node *attachedNode = GetNode(node->in.attachedNodeHandle);
			// ImDrawLine(node->rect.pos, attachedNode->rect.pos);
		// }
	// }

	for(int i = 0; i < _nodeState->nodeAmount; i++) {
		Node *node = GetNode(i);
		if(!node->free)
			DrawNode(i);
	}

	if(_nodeState->isDragging) {
		switch(_nodeState->draggedElementType) {
			case EDITOR_ELEMENT_NODE: {
				if(!mouse_buttons[GLFW_MOUSE_BUTTON_LEFT]) {
					_nodeState->isDragging = false;
				}
				else {
					Node *node = GetNode(_nodeState->draggedNodeHandle);
					node->pos = mouse + _nodeState->dragOffset;
				}
				break;
			}

			case EDITOR_ELEMENT_INPUT: {
				if(!mouse_buttons[GLFW_MOUSE_BUTTON_LEFT]) {
					_nodeState->isDragging = false;
					if(hoverState.elementType == EDITOR_ELEMENT_OUTPUT) {
						if(hoverState.nodeHandle != _nodeState->draggedNodeHandle) {
							AttachNodeSockets(_nodeState->draggedNodeHandle,
											  _nodeState->draggedNodeIOIndex,
											  hoverState.nodeHandle,
											  hoverState.ioIndex);
						}
					}
					else {
						DeattachNodeSockets(_nodeState->draggedNodeHandle, _nodeState->draggedNodeIOIndex);
					}
				}
				else {
					Rect rect = GetNodeInputRect(_nodeState->draggedNodeHandle, _nodeState->draggedNodeIOIndex);
					ImDrawLine(rect.pos, mouse);
				}
				break;
			}

			case EDITOR_ELEMENT_OUTPUT: {
				if(!mouse_buttons[GLFW_MOUSE_BUTTON_LEFT]) {
					_nodeState->isDragging = false;
				}
				else {
					Rect rect = GetNodeOutputRect(_nodeState->draggedNodeHandle, _nodeState->draggedNodeIOIndex);
					ImDrawLine(rect.pos, mouse);
				}
					
				break;
			}

			default: {
				break;
			}
				
		}
	}

	for(int i = 0; i < _nodeState->nodeAmount; i++) {
		Node *node = &_nodeState->nodes[i];
		node->function(node);
	}
}

void NodeEditorCleanup()
{
	free(_nodeState->nodes);
	free(_nodeState);
}
