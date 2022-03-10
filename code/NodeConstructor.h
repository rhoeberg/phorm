#pragma once

typedef ObjectHandle (*NodeCreateFunc)();
typedef void (*NodeSetupFunc)(Node *self);

void BaseNodeDrawFunc(Node *node);
void AddNodeConstructor(String name, NodeOp op, NodeCreateFunc createFunc, NodeSetupFunc setupFunc = NULL);

struct NodeConstructor
{
	NodeOp op;
	NodeSetupFunc setupFunc;
	NodeCreateFunc createFunc;
	bool noCache;
};

struct NodeConstructorState
{
	PArray<String> names;
	PMap<String, NodeConstructor> constructors;
	vec2 nextPos;
};

PArray<String>* GetNodeNames();
PMap<String, NodeConstructor>* GetNodeConstructors();
void SetNextConstructPos(vec2 pos);
ObjectHandle ConstructNode(String name, NodeConstructor *nodeConstructor);
PArray<String>* GetNodeNames();
void NamesBeginningWith(PArray<String> *array, String typed);
void InitializeNodeConstructors();
void SetupNodeConstructors();
void AddNodeConstructor(String name, NodeConstructor constructor);

#include "nodes/LoadTextureNode.h"
#include "nodes/NoiseBitmap.h"
#include "nodes/BlurNode.h"
#include "nodes/WaveyNode.h"
#include "nodes/MixTextureNode.h"
#include "nodes/VideoNode.h"
#include "nodes/SceneRenderNode.h"
#include "nodes/SceneNode.h"
#include "nodes/CubeNode.h"
#include "nodes/GridNode.h"
#include "nodes/MeshNoise.h"
#include "nodes/ParticleNode.h"
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
#include "nodes/OSCNode.h"
#include "nodes/MulNode.h"
#include "nodes/DoubleNode.h"
#include "nodes/AddNode.h"
#include "nodes/IntToDouble.h"
#include "nodes/MidiCCNode.h"
#include "nodes/DoubleToInt.h"
#include "nodes/RandIntNode.h"
#include "nodes/TickNode.h"
#include "nodes/CameraNode.h"
#include "nodes/CameraSwitch.h"

// output node
#include "nodes/OutputNode.h"

global NodeConstructorState *_nodeConstructorState;
