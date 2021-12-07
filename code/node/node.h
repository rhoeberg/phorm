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

#include "texture.h"
#include "mesh.h"
#include "renderobject.h"

enum NodeType {
	NIL_NODE,
	TEXTURE_NODE,
	MESH_NODE,
	RENDEROBJECT_NODE,
	DOUBLE_NODE,
};

struct DataHandle {
	unsigned int id;
	NodeType type;
};

struct NodeHandle {
	unsigned int id;
	NodeType type;

	NodeHandle() {
		type = NIL_NODE;
	}
};

struct NodeInput {
	NodeType type;
	NodeHandle handle;
	bool isset;

	NodeInput(NodeType _type) {
		type = _type;
		isset = false;
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
typedef void(*NodeOperation)(Node *self);

struct Node {
	NodeOperation op;
	NodeType type; //defines the return of the node operation
	vec2 pos;
	bool changed;
	char name[128];
	VMArray<NodeInput> inputs;
	VMArray<NodeParameter> params;
	int extraHandle;
	bool initialized;

	void AddInput(NodeType type)
	{
		NodeInput input = {};
		input.type = type;
		input.handle = {};
		inputs.Insert(input);
	}

	void SetDataHandle(DataHandle handle) { dataHandle = handle; }

	DataHandle GetDataLast() { return dataHandle; }
	DataHandle GetData();

private:
	DataHandle dataHandle;
};
