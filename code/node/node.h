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

struct NodeInput {
	DataType type;
	ObjectHandle handle;
	bool handleIsset;

	NodeInput(DataType _type) {
		type = _type;
		handleIsset = false;
	}

	NodeInput() {}
};

/* enum NodeParameterType { */
/* 	PARAM_INT, */
/* 	PARAM_DOUBLE, */
/* 	PARAM_VEC3, */
/* 	PARAM_STRING, */
/* }; */

/* struct Node; */
/* typedef void(*NodeOperation)(Node *self); */
/* typedef void(*NodeDrawingFunction)(Node *self); */
enum NodeOp;
enum NodeDrawFunc;

/* struct ParamList { */
	/* NodeParameter params[MAX_PARAMETERS]; */
/* }; */

struct Node {
	/* NodeOperation op; */
	/* NodeDrawingFunction drawingFunction; */
	NodeOp op;
	NodeDrawFunc drawFunc;
	DataType type; //defines the return of the node operation
	Rect rect;
	bool changed;
	/* uint8_t changeVer; */
	/* uint8_t lastChangeVer; */
	char name[128];
	/* VMArray<NodeInput> inputs; */
	/* VMArray<NodeParameter> params; */
	FixedArray<NodeInput> inputs;
	FixedArray<NodeParameter> params;
	/* int extraHandle; */
	ObjectHandle extraHandle;
	bool initialized;

	void AddInput(DataType type)
	{
		NodeInput input = {};
		input.type = type;
		input.handle = {};
		inputs.Insert(input);
	}

	void SetDataHandle(ObjectHandle handle) { dataHandle = handle; }

	ObjectHandle GetDataLast() { return dataHandle; }
	ObjectHandle GetData();
	void CallOp();

	bool Changed();

private:
	ObjectHandle dataHandle;
};

ObjectHandle AddNode(const char *name, DataType type, NodeOp op, FixedArray<NodeParameter> params, FixedArray<NodeInput> inputs, ObjectHandle extraHandle = ObjectHandle(-1));
