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
  - cos
  - time (DONE)
  - sin (DONE)

 */


#include "node_editor.h"
#include <math.h>

#define NODE_WIDTH 40
#define NODE_HEIGHT 30
#define NODE_SOCKET_WIDTH 20
#define NODE_SOCKET_HEIGHT 10

void NodeEditorInitialize()
{
	_nodeState = (NodeState*)malloc(sizeof(NodeState));
	_nodeState->nodes = (Node*)malloc(sizeof(Node) * MAX_NODE_AMOUNT);
	_nodeState->nodeAmount = 0;
	_nodeState->isDragging = false;
}

// TODO (rhoe) change the function naming to something else
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

	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glEnable(GL_TEXTURE_2D);
	// glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_2D, ftex);

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

void DebugFunction(Node *self)
{
	self->outputs[0].data.v3.x = sin(glfwGetTime());
}

void TimeFunction(Node *self)
{
	self->outputs[0].data.s = glfwGetTime();
}

void SinFunction(Node *self)
{
	GLfloat s = 0.0f;
	if(self->inputs[0].attached) {
		Node *attachedNode = GetNode(self->inputs[0].nodeHandle);
		if(attachedNode != NULL) {
			int outputIndex = self->inputs[0].nodeOutputIndex;
			s = attachedNode->outputs[outputIndex].data.s;
		}
	}

	self->outputs[0].data.s = sin(s);
}

void Vec3Function(Node *self)
{
	// here we should be able to set the x y and z of the vec3
	GLfloat x = self->outputs[0].data.v3.x;
	GLfloat y = self->outputs[0].data.v3.y;
	GLfloat z = self->outputs[0].data.v3.z;

	if(self->inputs[0].attached) {
		Node *attachedNode = GetNode(self->inputs[0].nodeHandle);
		if(attachedNode != NULL) {
			int outputIndex = self->inputs[0].nodeOutputIndex;
			x = attachedNode->outputs[outputIndex].data.s;
		}
	}
	if(self->inputs[1].attached) {
		Node *attachedNode = GetNode(self->inputs[1].nodeHandle);
		if(attachedNode != NULL) {
			int outputIndex = self->inputs[1].nodeOutputIndex;
			y = attachedNode->outputs[outputIndex].data.s;
		}
	}
	if(self->inputs[2].attached) {
		Node *attachedNode = GetNode(self->inputs[2].nodeHandle);
		if(attachedNode != NULL) {
			int outputIndex = self->inputs[2].nodeOutputIndex;
			z = attachedNode->outputs[outputIndex].data.s;
		}
	}

	self->outputs[0].data.v3 = vec3(x, y, z);
}

// TODO (rhoe) we need some way to avoid problems with dangling handles
//             if old node slots are reused
int AddNode(char *name)
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
		node->pos = vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		// node->rect.width = 0.1f;
		// node->rect.height = 0.1f;

		for(int i = 0; i < ARRAY_SIZE(node->inputs); i++) {
			node->inputs[i].active = false;
			node->inputs[i].attached = false;
		}
		for(int i = 0; i < ARRAY_SIZE(node->outputs); i++) {
			node->outputs[i].active = false;
		}

		sprintf(node->name, "%s", name);
	}

	

	return result;
}

bool TryAddNodeInput(int handle, NodeDataType type)
{
	Node *node = GetNode(handle);

	bool foundFreeSlot = false;
	for(int i = 0; i < ARRAY_SIZE(node->inputs); i++) {
		if(!node->inputs[i].active) {
			node->inputs[i].active = true;
			node->inputs[i].type = type;
			foundFreeSlot = true;
			break;
		}
	}

	return foundFreeSlot;
}

// TODO (rhoe) we could overload this function to have one for each data type
bool TryAddNodeOutput(int handle, NodeDataType type, NodeBufferData data = {})
{
	Node *node = GetNode(handle);

	bool foundFreeSlot = false;
	for(int i = 0; i < ARRAY_SIZE(node->outputs); i++) {
		if(!node->outputs[i].active) {
			node->outputs[i].active = true;
			node->outputs[i].type = type;
			node->outputs[i].data = data;
			foundFreeSlot = true;
			break;
		}
	}

	return foundFreeSlot;
}

int AddCubeNode()
{
	int handle = AddNode("cube");
	_nodeState->nodes[handle].function = &CubeFunction;

	TryAddNodeInput(handle, DATA_TYPE_VEC3);

	return handle;
}

int AddDebugNode()
{
	int handle = AddNode("debug");
	_nodeState->nodes[handle].function = &DebugFunction;
	TryAddNodeOutput(handle, DATA_TYPE_VEC3);
	return handle;
}

int AddVec3Node()
{
	int handle = AddNode("vec3");
	_nodeState->nodes[handle].function = &Vec3Function;

	TryAddNodeInput(handle, DATA_TYPE_SCALAR);
	TryAddNodeInput(handle, DATA_TYPE_SCALAR);
	TryAddNodeInput(handle, DATA_TYPE_SCALAR);

	NodeBufferData data = {};
	data.v3 = vec3(0, 0, 0);
	TryAddNodeOutput(handle, DATA_TYPE_VEC3, data);

	return handle;
}

int AddSinNode()
{
	int handle = AddNode("sin");
	_nodeState->nodes[handle].function = &SinFunction;

	TryAddNodeInput(handle, DATA_TYPE_SCALAR);

	TryAddNodeOutput(handle, DATA_TYPE_SCALAR);

	return handle;
}

int AddTimeNode()
{
	int handle = AddNode("time");
	_nodeState->nodes[handle].function = &TimeFunction;

	TryAddNodeOutput(handle, DATA_TYPE_SCALAR);

	return handle;
}

Node* GetNode(int handle)
{
	if(handle > MAX_NODE_AMOUNT - 1) {
		return NULL;
	}

	if(handle > _nodeState->nodeAmount) {
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

	result.height = NODE_HEIGHT;
	result.width = NODE_WIDTH + (NODE_WIDTH * maxSize);

	return result;
}

Rect GetNodeInputRect(int nodeHandle, int inputIndex)
{
	Rect result = {};

	// Node *node = GetNode(nodeHandle);
	Rect nodeRect = GetNodeRect(nodeHandle);

	result.pos = nodeRect.pos + vec2(NODE_WIDTH * inputIndex, -NODE_SOCKET_HEIGHT);
	result.width = NODE_SOCKET_WIDTH;
	result.height = NODE_SOCKET_HEIGHT;

	return result;
}

Rect GetNodeOutputRect(int nodeHandle, int inputIndex)
{
	Rect result = {};

	// Node *node = GetNode(nodeHandle);
	Rect nodeRect = GetNodeRect(nodeHandle);

	result.pos = nodeRect.pos + vec2(NODE_WIDTH * inputIndex, nodeRect.height);
	result.width = NODE_SOCKET_WIDTH;
	result.height = NODE_SOCKET_HEIGHT;

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

int GetSelectedNodeHandle()
{
	return _nodeState->selectedNodeHandle;
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

	ImDrawSetColor(vec3(0, 0, 0));
	ImDrawLine(rect.pos, rect.pos + vec2(rect.width, 0));
	ImDrawLine(rect.pos + vec2(rect.width, 0), rect.pos + vec2(rect.width, rect.height));
	ImDrawLine(rect.pos + vec2(0, rect.height), rect.pos + vec2(rect.width, rect.height));
	ImDrawLine(rect.pos, rect.pos + vec2(0, rect.height));

	ImDrawText(rect.pos + vec2(5, rect.height - 5), node->name);
}

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
			_nodeState->selectedNodeHandle = hoverState.nodeHandle;
			_nodeState->draggedNodeHandle = hoverState.nodeHandle;
			_nodeState->draggedNodeIOIndex = hoverState.ioIndex;
			_nodeState->draggedElementType = hoverState.elementType;
			_nodeState->isDragging = true;
			Node *node = GetNode(hoverState.nodeHandle);
			_nodeState->dragOffset = node->pos - mouse;
		}
	}

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
					ImDrawSetColor(vec3(1, 1, 1));
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
					ImDrawSetColor(vec3(1, 1, 1));
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
