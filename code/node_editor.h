#pragma once

#include "math.h"
#include <vector>

struct Node;

typedef void(*NodeFunction)(Node *self);

struct NodeIOBuffer {
	vec3 *pos;
	Node *link;
	bool attached;
};

struct Node {
	NodeFunction function;
	Rect rect;
	bool dragged;

	NodeIOBuffer in;
	vec3 out;
};

struct NodeState {
	std::vector<Node> nodes; // TODO(rhoe) seems to be some funny pointer stuff going on with std vector (maybe when resizing)
	// should think about using another data structure

	bool isDragging;
	Node *draggedNode;
	vec2 dragOffset;
};

Node* AddNode(NodeState *nodeState);
Node* AddCube(NodeState *nodeState);
Node* AddNoneNode(NodeState *nodeState);
bool IsMouseOverNode(vec2 mouse, Node node);
void DrawNode(Node node);
void UpdateNodeEditor(NodeState *nodeState);
