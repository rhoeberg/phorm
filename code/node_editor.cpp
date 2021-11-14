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


 */

#include "node_editor.h"

// struct NodeIO {
// 	vec3 vInputs[12];
// };

// enum NodeLinkType {
// 	LINK_VEC3,
// };

// struct NodeLink {
// 	NodeLinkType type;
// 	Node *output;
// 	int outPar;
// 	int inPar;
// };

void CubeFunction(Node *self)
{
	vec3 pos = *self->in.pos;

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
	self->out.x = glfwGetTime();
}

// struct NodeDragState {
	// bool isDragging;
// };
Node* AddNode(NodeState *nodeState)
{
	Node node = {};
	node.rect.width = 0.1f;
	node.rect.height = 0.1f;
	
	nodeState->nodes.push_back(node);
	Node *result = &nodeState->nodes.data()[nodeState->nodes.size() - 1];
	// TODO(rhoe) be carefull about these pointers from a std vector

	return result;
}

Node* AddCube(NodeState *nodeState)
{
	Node* result = AddNode(nodeState);
	result->function = &CubeFunction;
	result->rect.width = 0.2f;

	return result;
}

Node* AddNoneNode(NodeState *nodeState)
{
	Node *node = AddNode(nodeState);
	node->function = &NoneFunction;
	return node;
}

bool IsMouseOverNode(vec2 mouse, Node *node)
{
	if(PointInsideRect(mouse, node->rect))
		return true;

	return false;
}

void DrawNode(Node *node)
{
	ImDrawRect(node->rect);
}

void UpdateNodeEditor(NodeState *nodeState)
{
	for(int i = 0; i < nodeState->nodes.size(); i++) {
		Node *node = &nodeState->nodes[i];
		if(IsMouseOverNode(mouse, node)) {
			ImDrawSetColor(vec3(1, 0, 0));
			if(mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] && !nodeState->isDragging) {
				nodeState->isDragging = true;
				nodeState->draggedNode = node;
				nodeState->dragOffset = node->rect.pos - mouse;
				// nodeState->nodes[i].dragging = false;
			}
		}
		else {
			ImDrawSetColor(vec3(0, 1, 0));
		}
		DrawNode(node);

		if(node->in.attached) {
			// draw the line
			ImDrawLine(node->rect.pos, node->in.link->rect.pos);
		}
	}

	if(nodeState->isDragging) {
		if(!mouse_buttons[GLFW_MOUSE_BUTTON_LEFT])
			nodeState->isDragging = false;
		else {
			nodeState->draggedNode->rect.pos = mouse + nodeState->dragOffset;
		}
	}

	for(int i = 0; i < nodeState->nodes.size(); i++) {
		Node *node = &nodeState->nodes[i];
		node->function(node);
	}
}
