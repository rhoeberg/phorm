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

  

  

  __ GENERALIZATION IDEA __  (not yet implemented)
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


  
  





  __ HANDLE SYSTEM __ (not yet implemented)
  we need unique handles pr node / sub node
  these handles is going to be indices in their respective arrays.
  
  problem:
  when removing a node its important that we dont move all elements of the node 
  arrays, this would invalidate all handles above the removed handle.
  
  for this reason we need to simply "deactivate" the array slot when removing a node
  this can be done either by having a bool in the node type which defines if its 
  free or not.

  another way to fix it could be to have a seperate parallel array with only bools
  which defines if its active.
  we then look in the "bool" array to find a free slot in the node array
  

  handles should be type specific
  for this just create a thin struct wrapper around the handle so we can differentiate
  the types

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

	NodeInput(NodeType _type) {
		type = _type;
	}

	NodeInput() {}
};

enum NodeParameterType {
	PARAM_INT,
	PARAM_DOUBLE,
	PARAM_VEC3,
	PARAM_STRING,
};

// TODO (rhoe) add a way to hide/expose node parameters from node editor
struct NodeParameter {
	NodeParameterType type;
	char name[128];
	bool exposed;

	union {
		int i;
		double d;
		vec3 v3;
		char str[128];
	};

	// TODO (rhoe) can we make a unified constructor that sets a default parameter?
	NodeParameter(const char *_name, NodeParameterType _type, int _i) {
		type = _type;
		sprintf(name, "%s", _name);
		i = _i;
	}

	NodeParameter(const char *_name, NodeParameterType _type, double _d) {
		type = _type;
		sprintf(name, "%s", _name);
		d = _d;
	}

	NodeParameter(const char *_name, NodeParameterType _type, vec3 _v3) {
		type = _type;
		sprintf(name, "%s", _name);
		v3 = _v3;
	}

	NodeParameter(const char *_name, NodeParameterType _type, const char *_str) {
		type = _type;
		sprintf(name, "%s", _name);
		sprintf(str, "%s", _str);
	}
};

struct Node;

typedef int(*NodeOperation)(Node *self);

struct Node {
	NodeOperation op;
	NodeType type; //defines the return of the node operation
	/* Rect rect; */
	vec2 pos;
	bool changed;
	char name[128];
	VMArray<NodeInput> inputs;
	VMArray<NodeParameter> params;

	void AddInput(NodeType type)
	{
		NodeInput input = {};
		input.type = type;
		input.handle = -1;
		inputs.Insert(input);
	}

	void SetDataHandle(int handle) { dataHandle = handle; }

	int GetDataLast() { return dataHandle; }

	int GetData()
	{
		if(changed) {
			changed = false;
			return op(this);
		}

		return dataHandle;
	}

private:
	int dataHandle;
};

struct Texture {
	Pixel pixels[PIXEL_AMOUNT];
};

struct NodeState {
	// base node array
	VMArray<Node> nodes;

	// data arrays
	VMArray<Texture> textures;
};

global NodeState *_nodeState;

int GetPixelIndex(int x, int y);
Node* GetNode(int handle);
int AddNode(const char *name, NodeType type, NodeOperation op, VMArray<NodeParameter> params, VMArray<NodeInput> inputs);
Texture* GetTexture(int handle);
Texture* GetTextureInput(NodeInput input);

#include "RenderObjectNode.h"
#include "BlurNode.h"
#include "LoadTextureNode.h"
#include "MixTextureNode.h"
