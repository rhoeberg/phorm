/*
  
  Objective oriented approach to the node system
  we have a base node class and from that we would make
  core node types like:
  - textureNode
  - renderObjectNode
  - materialNode
  - etc..
  

  using these core types as the base classes we can then create interesting
  nodes. like:
  - blur
  - slice
  - noise
  - etc..


  the core node types then provide a function (operation) which returns the type
  consumed by the next node in the chain.
  fx. a texture node provides a GetPixels function. 

  an example:
  a blur node is a texture node which have another texture node as input.
  when another node is then connected to the blur node and calls the GetPixels 
  function, the blur node then gets the pixels from its input by again calling
  GetPixels() and from that result it does the blur algorithm and returns that result
  futher down the chain of nodes.

  

  caching:
  we also have the possibility of caching the results if nothing in the chain above
  has changed.
  in our blur example this would mean that the first time its operation is executed
  it get the pixel from its input and does the algorithm, but the second time if
  nothing has changed in its input and nothing has changed in itself it simply returns
  the last calculated result.
 
  
 */

#pragma once

#include <vector>

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

class Node {
public:
	bool changed;
	bool dragging;
	Rect rect;
	char name[128];

	Node();
	void RenderNode();
	void Update();
	void SetName(const char *name);

	virtual void ShowGUI() = 0;
};

class TextureNode : public Node {
public:
	Pixel pixels[PIXEL_AMOUNT]; // maybe change this into a pointer?
	virtual Pixel* GetPixels() = 0;
};

class LoadTextureNode : public TextureNode {
public:
	char path[128];

	Pixel* GetPixels();
	void LoadPixels();
	void ShowGUI();
};

int GetPixelIndex(int x, int y)
{
	return ((y * TEXTURE_SIZE) + x);
}

class BlurTextureNode : public TextureNode {
public:
	TextureNode *input;
	int amount;

	BlurTextureNode();
	Pixel* GetPixels();
	void Blur();
	void ShowGUI();

};

class AddTextureNode : public TextureNode {
public:
	TextureNode *input1;
	TextureNode *input2;

	// 0 = input 1
	// 1 = input 2
	// 0.5 = equal mix
	float slider = 0.5f;

	Pixel* GetPixels();
	void AddInputs();
	void ShowGUI();
};

struct NodeState {
	VMArray<TextureNode> textureNodes;
	bool isDragging;
	Node *selectedNode = NULL;
};

global NodeState _nodeState;
