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

typedef void(*NodeFunction)(NodeInput in, NodeOutput out);

struct Node {
	NodeFunction function;
	/* NodeType type; */
	Rect rect;
	bool dragged;
	/* int id; */
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

void AddNode(NodeState *nodeState, Node node);
void AddCube(NodeState *nodeState);
void AddNoneNode(NodeState *nodeState);
bool IsMouseOverNode(vec2 mouse, Node node);
void DrawNode(Node node);
/* void UpdateCube(NodeCube *cube); */
/* NodeCube* GetCube(NodeState *nodeState, int id); */
void UpdateNodeEditor(NodeState *nodeState);
