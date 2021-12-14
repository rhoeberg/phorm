void InitializeNodes()
{
	_nodeState = (NodeState*)malloc(sizeof(NodeState));


	// TODO (rhoe) replace placement new
	// apparently we need to use "placement new" syntax here
	// because we want to call the constructor on a object
	// which has already been allocated with malloc
	// not very pretty syntax maybe we should just use a custom
	// init function instead of constructor

	// new(&_nodeState->nodes) VMArray<Node>();
	new(&_nodeState->nodes) ObjectContainer<Node>();

	// data containers
	// _nodeState->textures = VMArray<Texture>();
	// _nodeState->meshes = VMArray<Mesh>();
	// _nodeState->renderObjects = VMArray<RenderObject>();
	// _nodeState->videoNodes = VMArray<VideoNodeState>();
	// _nodeState->labelNodes = VMArray<LabelNodeState>();
	// _nodeState->doubles = VMArray<double>();

	new(&_nodeState->textures) VMArray<Texture>();
	new(&_nodeState->meshes) VMArray<Mesh>();
	new(&_nodeState->renderObjects) VMArray<RenderObject>();
	new(&_nodeState->videoNodes) VMArray<VideoNodeState>();
	new(&_nodeState->labelNodes) VMArray<LabelNodeState>();
	new(&_nodeState->doubles) VMArray<double>();

	new(&_nodeState->labels) HashMap<NodeHandle>();

	_nodeState->strings = VMArray<String>();
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

	return _nodeState->nodes.Get(handle);
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
		NOT_IMPLEMENTED
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
		node->params[paramIndex].nodeHandle = outHandle;
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

RenderObject CreateRenderObject()
{
	RenderObject result = {};

	result.VAOHandle = AddVAO();
	glGenBuffers(1, &result.EBO);
	glGenBuffers(1, &result.VBO);
	glGenTextures(1, &result.textureID);

	return result;
}

DataHandle AddNewRenderObject()
{
	RenderObject renderObject = CreateRenderObject();
	DataHandle handle = {};
	handle.type = DATA_RENDEROBJECT;
	handle.id = _nodeState->renderObjects.Insert(renderObject);
	return handle;
}

DataHandle AddString(char *value)
{
	String string = value;
	DataHandle handle = {};
	handle.type = DATA_STRING;

	// TODO (rhoe) think about how we free the strings
	handle.id = _nodeState->strings.Insert(string);
	return handle;
}

String* GetString(DataHandle handle)
{
	if(handle.type != DATA_STRING ||
	   handle.id < 0 ||
	   handle.id > _nodeState->strings.Count()) {
		return NULL;
		NOT_IMPLEMENTED
	}

	return &_nodeState->strings[handle.id];
}

void CleanupNodes()
{
	// TODO (rhoe) freeing moar stuff
	// we should also free all the arrays in the node
	// like input and param
	_nodeState->nodes.Free();
	_nodeState->textures.Free();

	for(int i = 0; i < _nodeState->strings.Count(); i++) {
		_nodeState->strings[i].Free();
	}
	_nodeState->strings.Free();

	free(_nodeState);
}

