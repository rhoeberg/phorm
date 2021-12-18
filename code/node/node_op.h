#pragma once

void BaseNodeDrawFunction(Node *node);

#include "BlurNode.h"
#include "LoadTextureNode.h"
#include "MixTextureNode.h"
#include "CubeNode.h"
#include "GridNode.h"
#include "MeshNoise.h"
#include "VideoNode.h"
#include "renderobject_node.h"
#include "TimeNode.h"
#include "SinNode.h"
#include "PointLightNode.h"
#include "Vec3Node.h"
#include "MulNode.h"
#include "DoubleNode.h"
#include "AddNode.h"
/* #include "LabelNode.h" */

/* typedef ObjectHandle (*NodeCreateExtraHandleFunc)(); */
typedef ObjectHandle (*NodeCreateFunc)(String name, vec2 pos, DataType dataType, NodeOp op, NodeDrawFunc drawFunc);

struct NodeConstructor
{
	NodeOp op;
	NodeDrawFunc drawFunc;
	DataType dataType;

	NodeCreateFunc createFunc;

	// TODO (rhoe) maybe we can come up with something prettier than this function pointer
	// just for adding a extraHandle to the node?
	/* NodeCreateExtraHandleFunc createExtraHandleFunc; */

	vec2 pos;

	FixedArray<NodeParameter> params;
	FixedArray<NodeInput> inputs;
};

global VMArray<String> nodeNames = VMArray<String>();;
global HashMap<NodeConstructor> nodeConstructors = HashMap<NodeConstructor>(1024);;
global vec2 nextConstructPos;

void SetNextConstructPos(vec2 pos)
{
	nextConstructPos = pos;
}

void ConstructNode(String name, NodeConstructor *nodeConstructor)
{
	/* ObjectHandle extraHandle = ObjectHandle(); */
	/* if(nodeConstructor->createExtraHandleFunc) { */
		/* extraHandle = nodeConstructor->createExtraHandleFunc(); */
	/* } */

	nodeConstructor->createFunc(name, nextConstructPos, nodeConstructor->dataType, nodeConstructor->op, nodeConstructor->drawFunc);
	/* AddNode(name.buffer, nextConstructPos, nodeConstructor->dataType, nodeConstructor->op, nodeConstructor->drawFunc, nodeConstructor->params, nodeConstructor->inputs, extraHandle); */
}

void AddNodeConstructor(String name, DataType dataType, NodeOp op, NodeDrawFunc drawFunc, NodeCreateFunc createFunc)
{

	nodeNames.Insert(name);
	NodeConstructor constructor = {};
	constructor.dataType = dataType;
	constructor.op = op;
	constructor.drawFunc = drawFunc;
	constructor.createFunc = createFunc;
	/* constructor.params = params; */
	/* constructor.inputs = inputs; */
	/* constructor.createExtraHandleFunc = createExtraHandleFunc; */
	nodeConstructors.Insert(name, constructor);
}

void AddNodeConstructor(String name, DataType dataType, NodeOp op, NodeCreateFunc createFunc)
{
	AddNodeConstructor(name, dataType, op, BaseNodeDrawFunction, createFunc);
}

void AddNodeConstructors()
{
	nextConstructPos = vec2(0, 0);
	AddNodeConstructor(String("blur texture"), DATA_TEXTURE, BlurOperation, CreateBlurTexture);
	AddNodeConstructor(String("mix texture"), DATA_TEXTURE, MixTextureOperation, CreateMixTexture);
	AddNodeConstructor(String("load texture"), DATA_TEXTURE, LoadTextureOperation, CreateLoadTexture);
	AddNodeConstructor(String("video"), DATA_TEXTURE, VideoOperation, CreateVideoNode);
	AddNodeConstructor(String("cube mesh"), DATA_MESH, CubeOperation, CreateCubeNode);
	AddNodeConstructor(String("grid mesh"), DATA_MESH, GridOperation, CreateGridNode);
	AddNodeConstructor(String("noise mesh"), DATA_MESH, MeshNoiseOperation, CreateMeshNoise);
	AddNodeConstructor(String("double"), DATA_DOUBLE, DoubleOperation, CreateDoubleNode);
	AddNodeConstructor(String("sin"), DATA_DOUBLE, SinOperation, CreateSinNode);
	AddNodeConstructor(String("mul"), DATA_DOUBLE, MulOperation, CreateMulNode);
	AddNodeConstructor(String("add"), DATA_DOUBLE, AddOperation, CreateAddNode);
	AddNodeConstructor(String("time"), DATA_DOUBLE, TimeOperation, DrawTimeNode, CreateTimeNode);
	AddNodeConstructor(String("renderobject"), DATA_RENDEROBJECT, RenderObjectOperation, CreateRenderObjectNode);
	AddNodeConstructor(String("pointlight"), DATA_POINTLIGHT, PointLightOperation, CreatePointLightNode);
	AddNodeConstructor(String("vec3"), DATA_VEC3, Vec3NodeOperation, CreateVec3Node);
}

VMArray<String> NamesBeginningWith(String typed)
{
	VMArray<String> results = VMArray<String>();
	for(i32 i = 0; i < nodeNames.Count(); i++) {
		if(nodeNames[i].length >= typed.length) {
			bool match = true;
			for(i32 j = 0; j < typed.length; j++) {
				// TODO (rhoe) should probably be case insensitive
				if(typed.buffer[j] != nodeNames[i].buffer[j]) {
					match = false;
					break;
				}
			}
			if(match) {
				results.Insert(nodeNames[i]);
			}
		}
	}

	return results;
}

void BaseNodeDrawFunction(Node *node)
{
	// Rect rect = GetNodeRect(handle);
	node->rect.width = NODE_BASE_WIDTH;
	node->rect.width += node->params.Count() * PARAM_WIDTH;
	node->rect.width += node->inputs.Count() * PARAM_WIDTH;
	node->rect.height = NODE_HEIGHT;
	// Rect rect = node->rect;
	ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
	ImDrawRect(node->rect);

	vec2 namePos = node->rect.pos + vec2(8.0f, node->rect.height - 8.0f);
	ImDrawText(namePos, node->name);
}
