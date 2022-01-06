/*
  
ideas for new nodes:
- mirror effect (takes a renderobject and mirrors position?)

 */

#pragma once

void BaseNodeDrawFunction(Node *node);

#include "nodes/BlurNode.h"
#include "nodes/LoadTextureNode.h"
#include "nodes/MixTextureNode.h"
#include "nodes/CubeNode.h"
#include "nodes/GridNode.h"
#include "nodes/MeshNoise.h"
#include "nodes/VideoNode.h"
#include "nodes/renderobject_node.h"
#include "nodes/CombineObjectsNode.h"
#include "nodes/MirrorNode.h"
#include "nodes/TimeNode.h"
#include "nodes/SinNode.h"
#include "nodes/SinWaveNode.h"
#include "nodes/SawWaveNode.h"
#include "nodes/HistogramNode.h"
#include "nodes/PointLightNode.h"
#include "nodes/Vec3Node.h"
#include "nodes/MulNode.h"
#include "nodes/DoubleNode.h"
#include "nodes/AddNode.h"
/* #include "LabelNode.h" */

typedef ObjectHandle (*NodeCreateFunc)(String name, DataType dataType, NodeOp op, NodeDrawFunc drawFunc);

struct NodeConstructor
{
	NodeOp op;
	NodeDrawFunc drawFunc;
	DataType dataType;
	NodeCreateFunc createFunc;
	vec2 pos;

	FixedArray<NodeParameter> params;
	FixedArray<NodeInput> inputs;
};

struct NodeConstructorState
{
	VMArray<String> names;
	HashMap<NodeConstructor> constructors;
	vec2 nextPos;
};

/* global VMArray<String> nodeNames = VMArray<String>();; */
/* global HashMap<NodeConstructor> nodeConstructors = HashMap<NodeConstructor>(1024);; */
/* global vec2 nextConstructPos; */

VMArray<String>* GetNodeNames();
HashMap<NodeConstructor>* GetNodeConstructors();
void SetNextConstructPos(vec2 pos);
void ConstructNode(String name, NodeConstructor *nodeConstructor);
VMArray<String>* GetNodeNames();
VMArray<String> NamesBeginningWith(String typed);
void InitializeNodeConstructors();

global NodeConstructorState *_nodeConstructorState;
