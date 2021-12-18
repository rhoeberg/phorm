void InitializeNodes()
{
	_nodeState = (NodeState*)malloc(sizeof(NodeState));


	// TODO (rhoe) replace placement new
	// apparently we need to use "placement new" syntax here
	// because we want to call the constructor on a object
	// which has already been allocated with malloc
	// not very pretty syntax maybe we should just use a custom
	// init function instead of constructor

	new(&_nodeState->nodes) ObjectContainer<Node>(HANDLE_NODE);

	// data containers
	new(&_nodeState->textures) ObjectContainer<Texture>(HANDLE_DATA, DATA_TEXTURE);
	new(&_nodeState->meshes) ObjectContainer<Mesh>(HANDLE_DATA, DATA_MESH);
	new(&_nodeState->renderObjects) ObjectContainer<RenderObject>(HANDLE_DATA, DATA_RENDEROBJECT);
	new(&_nodeState->videoNodes) ObjectContainer<VideoNodeState>(HANDLE_DATA, DATA_VIDEO_STATE);
	new(&_nodeState->doubles) ObjectContainer<double>(HANDLE_DATA, DATA_DOUBLE);
	new(&_nodeState->vec3s) ObjectContainer<vec3>(HANDLE_DATA, DATA_VEC3);
	new(&_nodeState->strings) ObjectContainer<String>(HANDLE_DATA, DATA_STRING);
	new(&_nodeState->pointLights) ObjectContainer<PointLight>(HANDLE_DATA, DATA_POINTLIGHT);

	// new(&_nodeState->labels) HashMap<NodeHandle>();
}

bool NodeExists(ObjectHandle *handle)
{
	return _nodeState->nodes.Exists(handle);
}

Node* GetNode(ObjectHandle *handle)
{
	if(!NodeExists(handle)) {
		return NULL;
	}

	return _nodeState->nodes.Get(handle);
}

void DeleteNode(ObjectHandle *handle)
{
	_nodeState->nodes.Remove(handle);
}

double* GetDouble(ObjectHandle *handle)
{
	return _nodeState->doubles.Get(handle);
}

double* GetDoubleOutput(ObjectHandle *handle)
{
	Node *node = GetNode(handle);
	if(!NodeExists(handle) ||
	   node->type != DATA_DOUBLE) {
		return NULL;
	}

	return GetDouble(&node->GetData());
}

vec3* GetVec3(ObjectHandle *handle)
{
	return _nodeState->vec3s.Get(handle);
}

Texture* GetTexture(ObjectHandle *handle)
{
	return _nodeState->textures.Get(handle);
}

Texture* GetTextureInput(NodeInput input)
{
	Node *inputNode = GetNode(&input.handle);

	if(inputNode == NULL ||
	   input.type != DATA_TEXTURE ||
	   inputNode->GetDataLast().id > _nodeState->textures.Count() - 1 ||
	   inputNode->GetDataLast().id < 0) {
		return NULL;
	}

	Texture *texture = GetTexture(&inputNode->GetData());
	return texture;
}

Mesh* GetMesh(ObjectHandle *handle)
{
	return _nodeState->meshes.Get(handle);
}

Mesh* GetMeshInput(NodeInput input)
{
	Node *inputNode = GetNode(&input.handle);

	if(inputNode == NULL ||
	   input.type != DATA_MESH ||
	   inputNode->GetDataLast().id > _nodeState->meshes.Count() - 1 ||
	   inputNode->GetDataLast().id < 0) {
		return NULL;
	}
	   
	return GetMesh(&inputNode->GetData());
}

RenderObject* GetRenderObject(ObjectHandle *handle)
{
	return _nodeState->renderObjects.Get(handle);
}

PointLight* GetPointLight(ObjectHandle *handle)
{
	return _nodeState->pointLights.Get(handle);
}

bool ConnectNodeParameter(ObjectHandle *handle, ObjectHandle *outHandle, int paramIndex)
{
	Node *node = GetNode(handle);
	Node *outputNode = GetNode(outHandle);

	if(!node || !outputNode) {
		return false;
	}

	if(handle->id == outHandle->id ||
	   node->params[paramIndex].type != outputNode->type) {
		return false;
	}

	if(node != NULL) {
		node->changed = true;
		node->params[paramIndex].nodeHandle = *outHandle;
	}

	return true;
}

bool ConnectNodeInput(ObjectHandle *handle, ObjectHandle *outHandle, int inputIndex)
{
	Node *inputNode = GetNode(handle);
	Node *outputNode = GetNode(outHandle);

	if(!inputNode || !outputNode) {
		return false;
	}

	if(handle->id == outHandle->id ||
	   inputNode->inputs[inputIndex].type != outputNode->type) {
		return false;
	}

	if(inputNode != NULL) {
		inputNode->changed = true;
		inputNode->inputs[inputIndex].handle = *outHandle;
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

ObjectHandle AddNewRenderObject()
{
	RenderObject renderObject = CreateRenderObject();
	return _nodeState->renderObjects.Insert(renderObject);
}

ObjectHandle AddString(char *value)
{
	String string = value;
	return _nodeState->strings.Insert(string);
}

String* GetString(ObjectHandle *handle)
{
	return _nodeState->strings.Get(handle);
}

void CleanupNodes()
{
	// TODO (rhoe) freeing moar stuff
	// we should also free all the arrays in the node
	// like input and param
	_nodeState->nodes.Free();
	_nodeState->textures.Free();

	for(int i = 0; i < _nodeState->strings.Count(); i++) {
		ObjectHandle handle = _nodeState->strings.GetHandle(i);
		String *string = _nodeState->strings.Get(&handle);
		string->Free();
	}
	_nodeState->strings.Free();

	free(_nodeState);
}

