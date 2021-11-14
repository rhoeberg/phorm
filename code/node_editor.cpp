#include "node_editor.h"



// struct NodeDragState {
	// bool isDragging;
// };
void AddNode(NodeState *nodeState, Node node)
{
	nodeState->nodes.push_back(node);
}

void AddCube(NodeState *nodeState)
{
	Node node = {};
	node.id = nodeState->cubeAmount;
	node.type = NODE_CUBE;
	node.rect.pos = vec2(0, 0);
	node.rect.width = 0.1f;
	node.rect.height = 0.1f;
	AddNode(nodeState, node);

	NodeCube cube = {};
	cube.pos = vec3(0, 0, 0);
	cube.id = nodeState->cubeAmount;
	nodeState->cubes.push_back(cube);

	nodeState->cubeAmount++;
}

void AddNoneNode(NodeState *nodeState)
{
	Node node = {};
	node.id = 0;
	node.type = NODE_NONE;
	AddNode(nodeState, node);
}

bool IsMouseOverNode(vec2 mouse, Node node)
{
	if(PointInsideRect(mouse, node.rect))
		return true;

	return false;
}

void DrawNode(Node node)
{
	ImDrawRect(node.rect);
}

void UpdateCube(NodeCube *cube)
{
	// render cube
	glUseProgram(shaderProgram);
	glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	model = glm::translate(model, glm::vec3(0, 0, 0));
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0, 1, 0));
	GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glUseProgram(0);
}

NodeCube* GetCube(NodeState *nodeState, int id)
{
	for(int i = 0; i < nodeState->cubes.size(); i++) {
		if(nodeState->cubes[i].id == id) {
			return &nodeState->cubes[i];
		}
	}

	return NULL;
}

void UpdateNodeEditor(NodeState *nodeState)
{
	for(int i = 0; i < nodeState->nodes.size(); i++) {
		if(IsMouseOverNode(mouse, nodeState->nodes[i])) {
			ImDrawSetColor(vec3(1, 0, 0));
			if(mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] && !nodeState->isDragging) {
				nodeState->isDragging = true;
				nodeState->draggedNode = &nodeState->nodes[i];
				nodeState->dragOffset = nodeState->nodes[i].rect.pos - mouse;
				// nodeState->nodes[i].dragging = false;
			}
		}
		else {
			ImDrawSetColor(vec3(0, 1, 0));
		}
		DrawNode(nodeState->nodes[i]);
	}

	if(nodeState->isDragging) {
		if(!mouse_buttons[GLFW_MOUSE_BUTTON_LEFT])
			nodeState->isDragging = false;
		else {
			// vec2 dir = nodeState->dragPos - mouse;
			nodeState->draggedNode->rect.pos = mouse + nodeState->dragOffset;
			// nodeState->dragPos = mouse;
		}
	}

	for(int i = 0; i < nodeState->nodes.size(); i++) {
		Node *node = &nodeState->nodes[i];

		switch(node->type) {
			case NODE_CUBE: {
				// handle cube
				NodeCube *cube = GetCube(nodeState, node->id);
				UpdateCube(cube);
				break;
			}
			case NODE_NONE: {
				break;
			}
		}
	}
													 
	// Node node = {};
	// node.rect.pos = mouse;
	// node.rect.width = 0.1f;
	// node.rect.height = 0.1f;
	// DrawNode(node);
}
