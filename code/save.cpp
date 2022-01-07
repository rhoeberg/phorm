struct SaveFile {
	FILE *file;
};

template <typename T>
void SaveVMArray(VMArray<T> *array, SaveFile *saveFile)
{
	// save vertices
	int count = array->Count();
	int max = array->Max();

	fwrite(&count, sizeof(int), 1, saveFile->file);
	fwrite(&max, sizeof(int), 1, saveFile->file);
	fwrite(array->Data(), sizeof(T) * count, 1, saveFile->file);
}

/*
  This function assumes that the next piece of data in the file
  is a VMArray of type T
 */
template <typename T>
void LoadNextVMArray(VMArray<T> *array, SaveFile *saveFile)
{
	int count;
	int max;
	fread(&count, sizeof(int), 1, saveFile->file);
	fread(&max, sizeof(int), 1, saveFile->file);
	// TODO (rhoe) maybe its possibly to avoid this extra malloc
	T *data = (T*)malloc(sizeof(T) * count);
	fread(data, sizeof(T) * count, 1, saveFile->file);
	*array = VMArray<T>(max, count, data);
	free(data);
}

template <typename T>
void SaveObjectContainer(ObjectContainer<T> *container, SaveFile *saveFile)
{
	SaveVMArray<T>(&container->elements, saveFile);
	SaveVMArray<bool>(&container->isFree, saveFile);
	SaveVMArray<u32>(&container->slotID, saveFile);
}

template <typename T>
void LoadObjectContainer(ObjectContainer<T> *container, SaveFile *saveFile)
{
	LoadNextVMArray<T>(&container->elements, saveFile);
	LoadNextVMArray<bool>(&container->isFree, saveFile);
	LoadNextVMArray<u32>(&container->slotID, saveFile);
}

void SaveBool(bool b, SaveFile *saveFile)
{
	fwrite(&b, sizeof(bool), 1, saveFile->file);
}

void SaveI32(i32 i, SaveFile *saveFile)
{
	fwrite(&i, sizeof(i32), 1, saveFile->file);
}

void SaveU32(u32 u, SaveFile *saveFile)
{
	fwrite(&u, sizeof(u32), 1, saveFile->file);
}

int LoadI32(SaveFile *saveFile)
{
	int result;
	fread(&result, sizeof(int), 1, saveFile->file);
	return result;
}

bool LoadBool(SaveFile *saveFile)
{
	bool result;
	fread(&result, sizeof(bool), 1, saveFile->file);
	return result;
}

u32 LoadU32(SaveFile *saveFile)
{
	u32 result;
	fread(&result, sizeof(u32), 1, saveFile->file);
	return result;
}

void SaveString(String str, SaveFile *saveFile)
{
	SaveI32(str.length, saveFile);
	fwrite(str.buffer, str.length, 1, saveFile->file);
}

String LoadString(SaveFile *saveFile)
{
	int length = LoadI32(saveFile);
	char *buffer = (char*)malloc(length + 1);
	fread(buffer, length, 1, saveFile->file);
	buffer[length] = '\0';
	free(buffer);
	return String(buffer); 
}

void SaveNodes()
{
	SaveFile saveFile = {};
	saveFile.file = fopen("testsave.octo", "wb");

	//////////////
	// SAVE NODES
	SaveObjectContainer<Node>(&_nodeState->nodes, &saveFile);

	//////////////
	// SAVE DATA
	SaveI32(_nodeState->textures.Count(), &saveFile);
	SaveI32(_nodeState->meshes.Count(), &saveFile);
	SaveI32(_nodeState->renderObjects.Count(), &saveFile);
	SaveI32(_nodeState->renderObjectGroups.Count(), &saveFile);
	SaveI32(_nodeState->doubles.Count(), &saveFile);
	SaveI32(_nodeState->videoNodes.Count(), &saveFile);
	// SaveI32(_nodeState->labelNodes.Count(), &saveFile);

	//strings
	SaveI32(_nodeState->strings.Count(), &saveFile);
	for(int i = 0; i < _nodeState->strings.Count(); i++) {
		SaveString(_nodeState->strings.elements[i], &saveFile);
		SaveBool(_nodeState->strings.isFree[i], &saveFile);
		SaveU32(_nodeState->strings.slotID[i], &saveFile);
	}


	//////////////
	// SAVE SCENES
	SaveObjectContainer<SceneObject>(&_sceneEditorState->scene.sceneObjects, &saveFile);
	// SaveObjectContainer<ObjectHandle>(&_sceneEditorState->scene.pointLights, &saveFile);


	fclose(saveFile.file);
}

void LoadNodes()
{
	SaveFile saveFile = {};
	saveFile.file = fopen("testsave.octo", "rb");

	if(!saveFile.file) {
		NOT_IMPLEMENTED
		return;
	}

	// _nodeState->nodes = LoadNextVMArray<Node>(&saveFile);
	LoadObjectContainer<Node>(&_nodeState->nodes, &saveFile);

	// hook up node function pointers
	// this is needed because we cannot serialize function pointers since they might change
	for(i32 i = 0; i < _nodeState->nodes.Count(); i++) {
		ObjectHandle handle = _nodeState->nodes.GetHandle(i);
		Node *node = _nodeState->nodes.Get(handle);
		if(node) {
			String name = String(node->name);
			NodeConstructor *nodeConstructor = GetNodeConstructors()->Get(name);
			if(nodeConstructor) {
				node->op = nodeConstructor->op;
				node->drawFunc = nodeConstructor->drawFunc;
			}
		}
	}

	// mark all nodes as dirty after load so we can regenerate all resources
	for(int i = 0; i < _nodeState->nodes.Count(); i++) {
		ObjectHandle handle = _nodeState->nodes.GetHandle(i);
		Node *node = _nodeState->nodes.Get(handle);
		if(node) {
			node->changed = true;
			node->initialized = false;
		}
	}

	// load textures
	{
		_nodeState->textures.Clear();
		int count = LoadI32(&saveFile);
		for(int i = 0; i < count; i++) {
			_nodeState->textures.InsertNew();
		}
	}

	// load meshes
	{
		_nodeState->meshes.Clear();
		int count = LoadI32(&saveFile);
		for(int i = 0; i < count; i++) {
			Mesh mesh = {};
			_nodeState->meshes.Insert(mesh);
		}
	}

	// load renderobjects
	{
		_nodeState->renderObjects.Clear();

		// we need to re-add the base texture object for the viewer
		// after clearing the rendereobjects array
		// _viewerRenderState.baseTextureObject = AddNewRenderObject();
		CreateViewerTextureRenderObject();

		int count = LoadI32(&saveFile);
		for(int i = 0; i < count; i++) {
			AddNewRenderObject();
		}
	}

	// load renderobject groups
	{
		_nodeState->renderObjectGroups.Clear();
		int count = LoadI32(&saveFile);
		for(int i = 0; i < count; i++) {
			_nodeState->renderObjectGroups.Insert(RenderObjectGroup());
		}
	}

	// load doubles
	{
		_nodeState->doubles.Clear();
		int count = LoadI32(&saveFile);
		for(int i = 0; i < count; i++) {
			double value = 0.0;
			_nodeState->doubles.Insert(value);
		}
	}

	// load video nodes
	{
		_nodeState->videoNodes.Clear();
		int count = LoadI32(&saveFile);
		for(int i = 0; i < count; i++) {
			_nodeState->videoNodes.Insert(VideoNodeState());
		}
	}

	// // load label nodes
	// {
	// 	_nodeState->labelNodes.Clear();
	// 	int count = LoadI32(&saveFile);
	// 	for(int i = 0; i < count; i++) {
	// 		_nodeState->labelNodes.Insert(LabelNodeState());
	// 	}
	// }

	// load Strings
	{
		_nodeState->strings.Clear();
		int count = LoadI32(&saveFile);
		for(int i = 0; i < count; i++) {
			String str = LoadString(&saveFile);
			bool free = LoadBool(&saveFile);
			u32 slotID = LoadU32(&saveFile);
			_nodeState->strings.InsertRaw(str, free, slotID);
		}
	}

	//////////////
	// LOAD SCENES
	LoadObjectContainer<SceneObject>(&_sceneEditorState->scene.sceneObjects, &saveFile);
	// LoadObjectContainer<ObjectHandle>(&_sceneEditorState->scene.pointLights, &saveFile);

}
