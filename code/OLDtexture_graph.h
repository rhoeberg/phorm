/*
  3 different types of operations
  - generators
  - modifiers
  - adders (adds 2 things together)
  
  each node doesnt hold any data, to get the data at a specific step we 
  calcuate evauluate which evaluates down the stack.

  
  TODO
  - use handles for stacks
  


  node style "stacks"
  
  a node that creates the pixel array
  

 */
#pragma once

#define TEXTURE_SIZE 512
#define TEXTURE_CHANNEL_AMOUNT 4
#define TEXTURE_PIXEL_AMOUNT (TEXTURE_SIZE * TEXTURE_SIZE)
#define MAX_TEXTURE_NODES 32
#define MAX_TEXTURE_STACKS 32

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

typedef void(*TextureOperation)(Pixel *pixels);

struct TextureNode {
	TextureOperation operation;
	int typeHandle;
	char name[128];
};

/* struct TextureStack { */
/* 	// the core data being operated on down the stack */
/* 	/\* unsigned char pixels[TEXTURE_SIZE * TEXTURE_SIZE * TEXTURE_CHANNEL_AMOUNT]; *\/ */
/* 	Pixel pixels[TEXTURE_PIXEL_AMOUNT]; */

/* 	// the nodes of the stack */
/* 	TextureNode nodes[32]; */
/* 	int nodeCount; */

/* 	GLuint textureID; */
/* }; */

struct TextureGraphState {
	TextureStack stacks[MAX_TEXTURE_STACKS];
	int stackCount;

	Pixel pixelBackBuffer[TEXTURE_PIXEL_AMOUNT];

	GLuint viewerQuad;
};

struct TextureNode {
	Pixel pixels[TEXTURE_PIXEL_AMOUNT];
	GLuint textureID;
};



TextureStack* AddStack();
void ProcessStack(TextureStack *stack);

global TextureGraphState *_textureGraphState;
