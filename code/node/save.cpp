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

void SaveInt(int i, SaveFile *saveFile)
{
	fwrite(&i, sizeof(int), 1, saveFile->file);
}

int LoadInt(SaveFile *saveFile)
{
	int result;
	fread(&result, sizeof(int), 1, saveFile->file);
	return result;
}

void SaveString(String str, SaveFile *saveFile)
{
	SaveInt(str.length, saveFile);
	fwrite(str.buffer, str.length, 1, saveFile->file);
}

String LoadString(SaveFile *saveFile)
{
	int length = LoadInt(saveFile);
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
	SaveInt(_nodeState->textures.Count(), &saveFile);
	SaveInt(_nodeState->meshes.Count(), &saveFile);
	SaveInt(_nodeState->renderObjects.Count(), &saveFile);
	SaveInt(_nodeState->doubles.Count(), &saveFile);
	SaveInt(_nodeState->videoNodes.Count(), &saveFile);
	SaveInt(_nodeState->labelNodes.Count(), &saveFile);

	//strings
	SaveInt(_nodeState->strings.Count(), &saveFile);
	for(int i = 0; i < _nodeState->strings.Count(); i++) {
		SaveString(_nodeState->strings[i], &saveFile);
	}


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

	// mark all nodes as dirty after load so we can regenerate all resources
	for(int i = 0; i < _nodeState->nodes.Count(); i++) {
		NodeHandle handle = _nodeState->nodes.GetHandle(i);
		Node *node = _nodeState->nodes.Get(handle);
		if(node) {
			node->changed = true;
			node->initialized = false;
		}
	}

	// load textures
	{
		_nodeState->textures.Clear();
		int count = LoadInt(&saveFile);
		for(int i = 0; i < count; i++) {
			_nodeState->textures.InsertNew();
		}
	}

	// load meshes
	{
		_nodeState->meshes.Clear();
		int count = LoadInt(&saveFile);
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

		int count = LoadInt(&saveFile);
		for(int i = 0; i < count; i++) {
			AddNewRenderObject();
		}
	}

	// load doubles
	{
		_nodeState->doubles.Clear();
		int count = LoadInt(&saveFile);
		for(int i = 0; i < count; i++) {
			double value = 0.0;
			_nodeState->doubles.Insert(value);
		}
	}

	// load video nodes
	{
		_nodeState->videoNodes.Clear();
		int count = LoadInt(&saveFile);
		for(int i = 0; i < count; i++) {
			_nodeState->videoNodes.Insert(VideoNodeState());
		}
	}

	// load label nodes
	{
		_nodeState->labelNodes.Clear();
		int count = LoadInt(&saveFile);
		for(int i = 0; i < count; i++) {
			_nodeState->labelNodes.Insert(LabelNodeState());
		}
	}

	// load Strings
	{
		_nodeState->strings.Clear();
		int count = LoadInt(&saveFile);
		for(int i = 0; i < count; i++) {
			String str = LoadString(&saveFile);
			_nodeState->strings.Insert(str);
		}
	}


}
