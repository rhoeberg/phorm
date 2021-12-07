void InitializeNodes()
{
	_nodeState = (NodeState*)malloc(sizeof(NodeState));

	_nodeState->nodes = VMArray<Node>();

	// data containers
	_nodeState->textures = VMArray<Texture>();
	_nodeState->meshes = VMArray<Mesh>();
	_nodeState->renderObjects = VMArray<RenderObject>();
	_nodeState->videoNodes = VMArray<VideoNodeState>();
	_nodeState->doubles = VMArray<double>();
}

bool NodeExists(NodeHandle handle)
{
	if(handle.id > _nodeState->nodes.Count() - 1) {
		return false;
	}

	return true;
}

Node* GetNode(NodeHandle handle)
{
	if(!NodeExists(handle)) {
		return NULL;
	}

	return &_nodeState->nodes[handle.id];
}

double* GetDouble(DataHandle handle)
{
	if(handle.type == DATA_DOUBLE)
		return &_nodeState->doubles[handle.id];
	else
		return NULL;
}

double* GetDoubleOutput(NodeHandle handle)
{
	Node *node = GetNode(handle);
	if(!NodeExists(handle) ||
	   node->type != DATA_DOUBLE) {
		NOT_IMPLEMENTED
		return NULL;
	}

	return GetDouble(node->GetData());
}

Texture* GetTexture(DataHandle handle)
{
	if(handle.type == DATA_TEXTURE)
		return &_nodeState->textures[handle.id];
	else
		return NULL;
}

Texture* GetTextureInput(NodeInput input)
{
	Node *inputNode = GetNode(input.handle);

	if(inputNode == NULL ||
	   input.type != DATA_TEXTURE ||
	   inputNode->GetDataLast().id > _nodeState->textures.Count() - 1 ||
	   inputNode->GetDataLast().id < 0) {
		return NULL;
	}

	Texture *texture = GetTexture(inputNode->GetData());
	return texture;
}

Mesh* GetMesh(DataHandle handle)
{
	if(handle.type != DATA_MESH) {
		// TODO err
		return NULL;
	}
	
	return &_nodeState->meshes[handle.id];
}

Mesh* GetMeshInput(NodeInput input)
{
	Node *inputNode = GetNode(input.handle);

	if(inputNode == NULL ||
	   input.type != DATA_MESH ||
	   inputNode->GetDataLast().id > _nodeState->meshes.Count() - 1 ||
	   inputNode->GetDataLast().id < 0) {
		return NULL;
	}
	   
	return GetMesh(inputNode->GetData());
}

RenderObject* GetRenderObject(DataHandle handle)
{
	if(handle.type != DATA_RENDEROBJECT) {
		// TODO err
		return NULL;
	}
	
	return &_nodeState->renderObjects[handle.id];
}

bool ConnectNodeParameter(NodeHandle handle, NodeHandle outHandle, int paramIndex)
{
	Node *node = GetNode(handle);
	Node *outputNode = GetNode(outHandle);

	if(handle.id == outHandle.id ||
	   !NodeExists(handle) ||
	   !NodeExists(outHandle) ||
	   node->params[paramIndex].type != outputNode->type) {
		return false;
	}

	if(node != NULL) {
		node->changed = true;
		node->params[paramIndex].handle = outHandle;
		node->params[paramIndex].handleIsset = true;
	}

	return true;
}

bool ConnectNodeInput(NodeHandle handle, NodeHandle outHandle, int inputIndex)
{
	Node *inputNode = GetNode(handle);
	Node *outputNode = GetNode(outHandle);

	if(handle.id == outHandle.id ||
	   !NodeExists(handle) ||
	   !NodeExists(outHandle) ||
	   inputNode->inputs[inputIndex].type != outputNode->type) {
		return false;
	}

	if(inputNode != NULL) {
		inputNode->changed = true;
		inputNode->inputs[inputIndex].handle = outHandle;
		inputNode->inputs[inputIndex].handleIsset = true;
	}

	return true;
}

DataHandle AddNewRenderObject()
{
	RenderObject renderObject = {};

	renderObject.VAOHandle = AddVAO();
	glGenBuffers(1, &renderObject.EBO);
	glGenBuffers(1, &renderObject.VBO);
	glGenTextures(1, &renderObject.textureID);

	glGenTextures(1, &renderObject.textureID);

	DataHandle handle = {};
	handle.type = DATA_RENDEROBJECT;
	handle.id = _nodeState->renderObjects.Insert(renderObject);
	return handle;
}

void CleanupNodes()
{
	// TODO (rhoe) freeing moar stuff
	// we should also free all the arrays in the node
	// like input and param
	_nodeState->nodes.Free();
	_nodeState->textures.Free();

	free(_nodeState);
}

