#pragma once

#define TEXTURE_SIZE 512
#define TEXTURE_CHANNEL_AMOUNT 4
#define PIXEL_AMOUNT (TEXTURE_SIZE * TEXTURE_SIZE)

struct Pixel {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	Pixel() {
		r = g = b = a = 0;
	}

	Pixel(unsigned int _r, unsigned int _g, unsigned int _b, unsigned int _a) {
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
};


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
struct TextureNode;

typedef Pixel*(*TextureNodeOp)(TextureNode *self);

struct TextureNode {
	vec2 pos;
	TextureNodeOp GetPixels;
	bool changed;
	int dataHandle;
	Pixel pixels[PIXEL_AMOUNT];
	/* Node node; */

	// could use dynamic array here
	NodeInput inputs[4];
	int inputCount;
};

struct BlurNode {
	int amount;
};

struct LoadTextureNode {
	char path[128];
};

struct NodeState {
	TextureNode textureNodes[256];
	int textureNodeCount;

	BlurNode blurNodes[256];
	int blurNodeCount;

	LoadTextureNode loadTextureNodes[256];
	int loadTextureNodeCount;


	GLuint textureID;
	GLuint viewerQuad;



	// TESTING STUFF
	int blurNodeHandle;
};

global NodeState *_nodeState;

