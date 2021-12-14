#pragma once

enum NodeOp {
	OP_DOUBLE_TIME,
	OP_TEXTURE_LOAD,
	OP_TEXTURE_BLUR,
	OP_TEXTURE_MIX,
	OP_TEXTURE_VIDEO,
	OP_MESH_CUBE,
	OP_RENDEROBJECT,
	OP_LABEL,
};

/* enum NodeDrawFunc { */
	/* NODE_DRAW_DEFAULT, */
	/* NODE_DRAW_TIME, */
/* }; */

void BaseNodeDrawFunction(Node *node);
void CallNodeDrawFunction(Node *self);

/* #include "nodestate.h" */
#include "BlurNode.h"
#include "LoadTextureNode.h"
#include "MixTextureNode.h"
#include "CubeNode.h"
#include "VideoNode.h"
#include "renderobject_node.h"
#include "TimeNode.h"
#include "LabelNode.h"

void CallNodeOp(Node *self)
{
	switch(self->op) {
		// DOUBLE OP
		case OP_DOUBLE_TIME: {
			TimeOperation(self);
			break;
		}		

		// TEXTURE OPS
		case OP_TEXTURE_LOAD: {
			LoadTextureOperation(self);
			break;
		}
		case OP_TEXTURE_BLUR: {
			BlurOperation(self);
			break;
		}
		case OP_TEXTURE_MIX: {
			MixTextureOperation(self);
			break;
		}
		case OP_TEXTURE_VIDEO: {
			VideoOperation(self);
			break;
		}

		// MESH OPS
		case OP_MESH_CUBE: {
			CubeOperation(self);
			break;
		}

		// RENDEROBJECT OP
		case OP_RENDEROBJECT: {
			RenderObjectOperation(self);
			break;
		}


		// LABEL OP
		case OP_LABEL: {
			LabelOperation(self);
		}
	}
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

void CallNodeDrawFunction(Node *self)
{
	switch(self->op) {
		case OP_DOUBLE_TIME: {
			DrawTimeNode(self);
			break;
		}
		case OP_LABEL: {
			DrawLabelNode(self);
			break;
		}

		default: {
			BaseNodeDrawFunction(self);
			break;
		}
	}
}