#pragma once

void BaseNodeDrawFunc(Node *node);

#include "nodes/LoadTextureNode.h"
#include "nodes/BlurNode.h"
#include "nodes/WaveyNode.h"
#include "nodes/MixTextureNode.h"
#include "nodes/VideoNode.h"
#include "nodes/SceneRenderNode.h"
#include "nodes/SceneNode.h"
#include "nodes/CubeNode.h"
#include "nodes/GridNode.h"
#include "nodes/MeshNoise.h"
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
#include "nodes/IntToDouble.h"
#include "nodes/MidiCCNode.h"
#include "nodes/DoubleToInt.h"

// output node
#include "nodes/OutputNode.h"

typedef ObjectHandle (*NodeCreateFunc)();
typedef void (*NodeSetupFunc)(Node *self);

struct NodeConstructor
{
	NodeOp op;
	NodeSetupFunc setupFunc;
	NodeCreateFunc createFunc;
};

struct NodeConstructorState
{
	VMArray<String> names;
	HashMap<NodeConstructor> constructors;
	vec2 nextPos;
};

VMArray<String>* GetNodeNames();
HashMap<NodeConstructor>* GetNodeConstructors();
void SetNextConstructPos(vec2 pos);
ObjectHandle ConstructNode(String name, NodeConstructor *nodeConstructor);
VMArray<String>* GetNodeNames();
void NamesBeginningWith(VMArray<String> *array, String typed);
void InitializeNodeConstructors();
void SetupNodeConstructors();

global NodeConstructorState *_nodeConstructorState;
