void InitializeData()
{
	_nodeState = (NodeState*)malloc(sizeof(NodeState));

	// NODES
	new(&_nodeState->nodes) ObjectContainer<Node>(HANDLE_NODE);

	// DATA
	new(&_nodeState->bitmaps) ObjectContainer<Bitmap>(HANDLE_DATA, DATA_BITMAP);
	new(&_nodeState->meshes) ObjectContainer<Mesh>(HANDLE_DATA, DATA_MESH);
	new(&_nodeState->renderObjects) ObjectContainer<RenderObject>(HANDLE_DATA, DATA_RENDEROBJECT);
	new(&_nodeState->renderObjectGroups) ObjectContainer<RenderObjectGroup>(HANDLE_DATA, DATA_RENDEROBJECT_GROUP);
	new(&_nodeState->doubles) ObjectContainer<double>(HANDLE_DATA, DATA_DOUBLE);
	new(&_nodeState->ints) ObjectContainer<int>(HANDLE_DATA, DATA_INT);
	new(&_nodeState->vec3s) ObjectContainer<vec3>(HANDLE_DATA, DATA_VEC3);
	new(&_nodeState->vec4s) ObjectContainer<vec4>(HANDLE_DATA, DATA_VEC4);
	new(&_nodeState->strings) ObjectContainer<String>(HANDLE_DATA, DATA_STRING);
	new(&_nodeState->pointLights) ObjectContainer<PointLight>(HANDLE_DATA, DATA_POINTLIGHT);
	new(&_nodeState->scenes) ObjectContainer<Scene>(HANDLE_DATA, DATA_SCENE);

	// NODE SPECIFIC STATE
	new(&_nodeState->sceneRenderDatas) ObjectContainer<SceneRenderData>(HANDLE_DATA, DATA_NONE);
	new(&_nodeState->videoNodes) ObjectContainer<VideoNodeState>(HANDLE_DATA, DATA_NONE);
	new(&_nodeState->sinWaveNodes) ObjectContainer<SinWaveNodeState>(HANDLE_DATA, DATA_NONE);
	new(&_nodeState->bitmapComputeStates) ObjectContainer<BitmapComputeState>(HANDLE_DATA, DATA_NONE);
	new(&_nodeState->meshComputeStates) ObjectContainer<MeshComputeState>(HANDLE_DATA, DATA_NONE);
}

//////////
// Resource Access
//////////
ObjectContainer<Node>* GetNodes()
{
	return &_nodeState->nodes;
}

ObjectContainer<double>* GetDoubles()
{
	return &_nodeState->doubles;
}

ObjectContainer<int>* GetInts()
{
	return &_nodeState->ints;
}

ObjectContainer<Mesh>* GetMeshes()
{
	return &_nodeState->meshes;
}

ObjectContainer<Bitmap>* GetBitmaps()
{
	return &_nodeState->bitmaps;
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

ObjectContainer<vec4>* GetVec4s()
{
	return &_nodeState->vec4s;
}

ObjectContainer<PointLight>* GetPointLights()
{
	return &_nodeState->pointLights;
}

ObjectContainer<String>* GetStrings()
{
	return &_nodeState->strings;
}

ObjectContainer<Scene>* GetScenes()
{
	return &_nodeState->scenes;
}

ObjectContainer<SceneRenderData>* GetSceneRenderDatas()
{
	return &_nodeState->sceneRenderDatas;
}

ObjectContainer<BitmapComputeState>* GetBitmapComputeStates()
{
	return &_nodeState->bitmapComputeStates;
}

ObjectContainer<MeshComputeState>* GetMeshComputeStates()
{
	return &_nodeState->meshComputeStates;
}



//////////
// Resource Creation
//////////

ObjectHandle AddNewRenderObject()
{
	RenderObject renderObject = CreateRenderObject();
	return _nodeState->renderObjects.Insert(renderObject);
}

ObjectHandle AddString(const char *value)
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
	result.textureHandle = GFXTextureAdd();
	result.wireframe = false;

	return result;
}

//////////////////
// CLEANUP
/////////////////
void CleanupData()
{
	for(i32 i = 0; i < _nodeState->bitmaps.Count(); i++) {
		Bitmap *bitmap = _nodeState->bitmaps.GetAt(i);
		if(bitmap && bitmap->initialized) {
			free(bitmap->pixels);
		}
	}
}
