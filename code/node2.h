#pragma once

enum NodeType {
	TEXTURE_NODE,
};

struct NodeInput {
	NodeType type;
	int handle;
};

/* struct Node { */
  // basic node position data etc..
/* }; */

typedef Pixel*(*TextureNodeOp)(TextureNode *self);

struct TextureNode {
	TextureNodeOp op;
	bool changed;
	int dataHandle;
	Node node;

	// could use dynamic array here
	NodeInput inputs[4];
	int inputCount;
};

struct BlurNode {
	int amount;
};

struct LoadNode {
	char path[128];
};

struct NodeState {
	TextureNode textureNodes[256];
	int textureNodeCount;

	BlurNode blurNodes[256];
	int blurNodeCount;

	LoadNode LoadNodes[256];
	int loadNodeCount;
};

global NodeState *_nodeState;

