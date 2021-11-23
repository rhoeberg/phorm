#pragma once

struct RenderObject {

};

struct RenderObjectNode;

typedef RenderObject*(*RenderObjectNodeOp)(RenderObjectNode *self);

struct RenderObjectNode {
	RenderObject renderObject;

};
