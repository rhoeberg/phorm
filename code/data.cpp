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
	new(&_nodeState->renderObjectGroups) ObjectContainer<RenderObjectGroup>(HANDLE_DATA, DATA_RENDEROBJECT_GROUP);
	new(&_nodeState->doubles) ObjectContainer<double>(HANDLE_DATA, DATA_DOUBLE);
	new(&_nodeState->vec3s) ObjectContainer<vec3>(HANDLE_DATA, DATA_VEC3);
	new(&_nodeState->strings) ObjectContainer<String>(HANDLE_DATA, DATA_STRING);
	new(&_nodeState->pointLights) ObjectContainer<PointLight>(HANDLE_DATA, DATA_POINTLIGHT);

	// Specific node state
	new(&_nodeState->videoNodes) ObjectContainer<VideoNodeState>(HANDLE_DATA, DATA_VIDEO_STATE);
	new(&_nodeState->sinWaveNodes) ObjectContainer<SinWaveNodeState>(HANDLE_DATA, DATA_VIDEO_STATE);
	// new(&_nodeState->labels) HashMap<NodeHandle>();
}



//////////
// Resource Access
//////////

ObjectContainer<double>* GetDoubles()
{
	return &_nodeState->doubles;
}

ObjectContainer<Mesh>* GetMeshes()
{
	return &_nodeState->meshes;
}

ObjectContainer<Texture>* GetTextures()
{
	return &_nodeState->textures;
}

ObjectContainer<RenderObject>* GetRenderObjects()
{
	return &_nodeState->renderObjects;
}

ObjectContainer<RenderObjectGroup>* GetRenderObjectGroups()
{
	return &_nodeState->renderObjectGroups;
}

ObjectContainer<vec3>* GetVec3s()
{
	return &_nodeState->vec3s;
}

ObjectContainer<PointLight>* GetPointLights()
{
	return &_nodeState->pointLights;
}

ObjectContainer<String>* GetStrings()
{
	return &_nodeState->strings;
}



//////////
// Resource Creation
//////////

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

RenderObject CreateRenderObject()
{
	RenderObject result = {};

	result.VAOHandle = AddVAO();
	glGenBuffers(1, &result.EBO);
	glGenBuffers(1, &result.VBO);
	glGenTextures(1, &result.textureID);

	return result;
}
