#pragma once

#include "math.h"

#define MAX_NODE_AMOUNT 512

struct Node;

typedef void(*NodeFunction)(Node *self);

enum NodeDataType {
	DATA_TYPE_VEC3,
	DATA_TYPE_SCALAR,
};

union NodeBufferData {
	vec3 v3; //vector 3 type
	GLfloat s; // scalar type
};

struct NodeOutput {
	NodeDataType type;
	NodeBufferData data;
	bool active;
};

// TODO (rhoe) probably need some string name for the input
struct NodeInput {
	NodeDataType type;
	int nodeOutputIndex;
	int nodeHandle;

	bool attached;
	bool active;
};

struct Node {
	NodeFunction function;
	vec2 pos;
	/* Rect rect; */
	bool dragged;

	NodeInput inputs[4];
	NodeOutput outputs[4];

	char name[32];

	bool free;
};

enum NodeEditorElementType {
	EDITOR_ELEMENT_NODE,
	EDITOR_ELEMENT_INPUT,
	EDITOR_ELEMENT_OUTPUT,
};

struct NodeState {
	Node *nodes;
	int nodeAmount;

	bool isDragging;
	NodeEditorElementType draggedElementType;
	int draggedNodeHandle;
	int draggedNodeIOIndex;
	vec2 dragOffset;

	int selectedNodeHandle;
};

struct HoverState {
	int nodeHandle;
	int ioIndex;
	NodeEditorElementType elementType;
	bool hoveringElement;
};

int AddNode();
int AddCubeNode();
int AddDebugNode();
int AddVec3Node();
int AddSinNode();
int AddTimeNode();
bool IsMouseOverNode(vec2 mouse, Node node);
void DrawNode(Node node);
Node* GetNode(int handle);
void UpdateNodeEditor();
void CleanupNodeEditor();

NodeState *_nodeState;
