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
/* #include "LabelNode.h" */

typedef ObjectHandle (*NodeCreateExtraHandleFunc)();

struct NodeConstructor
{
	NodeOp op;
	NodeDrawFunc drawFunc;
	DataType dataType;

	// TODO (rhoe) maybe we can come up with something prettier than this function pointer
	// just for adding a extraHandle to the node?
	NodeCreateExtraHandleFunc createExtraHandleFunc;

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
	ObjectHandle extraHandle = ObjectHandle();
	if(nodeConstructor->createExtraHandleFunc) {
		extraHandle = nodeConstructor->createExtraHandleFunc();
	}

	AddNode(name.buffer, nextConstructPos, nodeConstructor->dataType, nodeConstructor->op, nodeConstructor->drawFunc, nodeConstructor->params, nodeConstructor->inputs, extraHandle);
}

void AddNodeConstructor(String name, DataType dataType, NodeOp op, NodeDrawFunc drawFunc, FixedArray<NodeParameter> params, FixedArray<NodeInput> inputs, NodeCreateExtraHandleFunc createExtraHandleFunc = NULL)
{

	nodeNames.Insert(name);
	NodeConstructor constructor = {};
	constructor.dataType = dataType;
	constructor.op = op;
	constructor.drawFunc = drawFunc;
	constructor.params = params;
	constructor.inputs = inputs;
	constructor.createExtraHandleFunc = createExtraHandleFunc;
	nodeConstructors.Insert(name, constructor);
}

void AddNodeConstructor(String name, DataType dataType, NodeOp op, FixedArray<NodeParameter> params, FixedArray<NodeInput> inputs, NodeCreateExtraHandleFunc createExtraHandleFunc = NULL)
{
	AddNodeConstructor(name, dataType, op, BaseNodeDrawFunction, params, inputs, createExtraHandleFunc);
}

void AddNodeConstructors()
{
	nextConstructPos = vec2(0, 0);

	AddNodeConstructor(String("blur texture"), DATA_TEXTURE, BlurOperation,
					   FixedArray<NodeParameter> {NodeParameter("amount", 20)},
					   FixedArray<NodeInput> {NodeInput(DATA_TEXTURE)});

	AddNodeConstructor(String("mix texture"), DATA_TEXTURE, MixTextureOperation,
					   FixedArray<NodeParameter> {NodeParameter("mix", 0.5)},
					   FixedArray<NodeInput> {
						   NodeInput(DATA_TEXTURE),
						   NodeInput(DATA_TEXTURE)});

	AddNodeConstructor(String("load texture"), DATA_TEXTURE, LoadTextureOperation,
					   FixedArray<NodeParameter> {NodeParameter("path", ""),},
					   FixedArray<NodeInput> {});

	
	{
		ObjectHandle extraHandle = _nodeState->videoNodes.Insert(VideoNodeState());
		AddNodeConstructor(String("video"), DATA_TEXTURE, VideoOperation,
						   FixedArray<NodeParameter> {NodeParameter("time", 0.0),},
						   FixedArray<NodeInput> {}, SetupVideoNode);
	}

	AddNodeConstructor(String("cube mesh"), DATA_MESH, CubeOperation,
					   FixedArray<NodeParameter>(), FixedArray<NodeInput>());

	AddNodeConstructor(String("grid mesh"), DATA_MESH, GridOperation,
					   FixedArray<NodeParameter>{
						   NodeParameter("width", 100),
						   NodeParameter("height", 100),
					   }, FixedArray<NodeInput>());

	AddNodeConstructor(String("noise mesh"), DATA_MESH, MeshNoiseOperation,
					   FixedArray<NodeParameter>{
						   NodeParameter("amount", 1.0),
						   NodeParameter("octaves", 1),
						   NodeParameter("persistance", 1.0),
						   NodeParameter("freq", 1.0),
					   }, FixedArray<NodeInput> {NodeInput(DATA_MESH)});

	AddNodeConstructor(String("sin"), DATA_DOUBLE, SinOperation,
					  FixedArray<NodeParameter>(),
					  FixedArray<NodeInput> { NodeInput(DATA_DOUBLE) });

	AddNodeConstructor(String("time"), DATA_DOUBLE, TimeOperation, DrawTimeNode,
					  FixedArray<NodeParameter>(),
					  FixedArray<NodeInput>());

	AddNodeConstructor(String("renderobject"), DATA_RENDEROBJECT, RenderObjectOperation,
					   FixedArray<NodeParameter> {
						   NodeParameter("pos", vec3(0, 0, 0)),
						   NodeParameter("label", "")},
					   FixedArray<NodeInput> {
						   NodeInput(DATA_MESH),
						   NodeInput(DATA_TEXTURE),
					   });

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

#define PARAM_WIDTH 40
#define PARAM_HEIGHT 20
#define NODE_HEIGHT 30
#define NODE_BASE_WIDTH 100

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

	vec2 namePos = node->rect.pos + vec2(10.0f, node->rect.height - 10.0f);
	ImDrawText(namePos, node->name);
}
