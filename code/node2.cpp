void InitializeNodes()
{
	_nodeState = (NodeState*)malloc(sizeof(NodeState));
}

int AddTextureNode()
{
	if(textureNodeCount < ARRAY_SIZE(textureNodes)) {
		int nodeHandle = _nodeState.textureNodeCount;
		textureNodeCount++;
		return nodeHandle;
	}
	return -1;
}

int AddBlurNode()
{
	int nodeHandle = AddTextureNode();
	if(nodeHandle > 0) {
		if(blurNodeCount < ARRAY_SIZE(blurNodeCount)) {
			int dataHandle = blurNodeCount;

			TextureNode *textureNode = &_nodeState->textureNodes[nodeHandle];
			BlurNode *blurNode = &_nodeState->blurNodes[dataHandle];
			textureNode->dataHandle = blurNodes[dataHandle];

			return nodeHandle;
		}
	}
	return -1;
}

void CleanupNodes()
{
	free(_nodeState);
}
