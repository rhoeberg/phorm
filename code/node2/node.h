/*
  
  A non OOP way of doing the node system.


  _data is structured like:_
  textureNodes[]
  blurNode[]
  ...
  
  RenderNodes[]
  renderNodeType[]
  ...
  

  we have a node type for each base data type:
  - texture
  - renderobject
  - ...
  
  each of these holds a function pointer specific to that type
  called an operation
  
  
  sub types are then stored in different arrays and can hold different
  data needed for that type. The sub type also defines the operation
  function pointer in the parent.
  - specific data
  - specific operation function

  examples of sub types:
  - blur
  - loadTexture

  

  

  _ GENERALIZATION IDEA _  (not yet implemented)
  we could further generalize the system by storing the data
  of each type seperately (we kind of do that in sub types)
  then we could get rid of the diffferent node types
  and have a single general node type with a single function pointer
  signature:
  typedef int(*NodeOperation)(Node *self)
  
  the returned int would then be a contextual handle to the 
  data type for that node.
  that means sometimes its a handle to a texture
  sometimes to a renderobject


  

 */

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

struct NodeHoverState {
	int nodeHandle;
	bool hoveringElement;
};

struct RenderObjectNode;
struct TextureNode;
struct BlurNode;
struct LoadTextureNode;

struct NodeState {
	VMArray<RenderObjectNode> renderObjectNodes;
	VMArray<TextureNode> textureNodes;
	VMArray<BlurNode> blurNodes;
	VMArray<LoadTextureNode> loadTextureNodes;

	NodeHoverState hoverState;

	bool isDragging;
	int draggedNodeHandle;

	// TESTING STUFF
	int blurNodeHandle;
	GLuint textureID;
	GLuint viewerQuad;
};

global NodeState *_nodeState;

int GetPixelIndex(int x, int y);

#include "RenderObjectNode.h"
#include "TextureNode.h"
#include "BlurNode.h"
#include "LoadTextureNode.h"
