#pragma once

#include "math.h"
#include <vector>

struct NodeInput {
};

struct NodeOutput {
};

enum NodeType {
	NODE_NONE,
	NODE_CUBE,
};

struct NodeVec3 {
	int id;

	vec3 v;
};

/* struct NodeCube { */
/* 	int id; */

/* 	// attributes */
/* 	vec3 pos; */

/* 	// inputs */


/* 	// outputs */
/* }; */
struct Node;

typedef void(*NodeFunction)(Node *self);

struct NodeIOBuffer {
	vec3 *pos;
	Node *link;
};

struct Node {
	NodeFunction function;
	/* NodeType type; */
	Rect rect;
	bool dragged;
	/* int id; */

	NodeIOBuffer in;
	vec3 out;
};

struct NodeState {
	std::vector<Node> nodes;
	bool isDragging;
	Node *draggedNode;
	vec2 dragOffset;

	// node types
	/* std::vector<NodeCube> cubes; */
	/* int cubeAmount; */
};

Node* AddNode(NodeState *nodeState);
Node* AddCube(NodeState *nodeState);
void AddNoneNode(NodeState *nodeState);
bool IsMouseOverNode(vec2 mouse, Node node);
void DrawNode(Node node);
/* void UpdateCube(NodeCube *cube); */
/* NodeCube* GetCube(NodeState *nodeState, int id); */
void UpdateNodeEditor(NodeState *nodeState);
