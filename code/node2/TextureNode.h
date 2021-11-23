#pragma once

struct TextureNode;

typedef Pixel*(*TextureNodeOp)(TextureNode *self);

struct TextureNode {
	Rect rect;
	TextureNodeOp GetPixels;
	bool changed;
	int dataHandle;
	Pixel pixels[PIXEL_AMOUNT];
	/* Node node; */

	VMArray<NodeInput> inputs;
};

int AddTextureNode()
{
	TextureNode *node = _nodeState->textureNodes.InsertNew();
	node->rect.pos = vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	node->rect.width = 100;
	node->rect.height = 40;
	node->changed = true;
	node->inputs = VMArray<NodeInput>();

	return _nodeState->textureNodes.Count() - 1; // get last inserted
}
