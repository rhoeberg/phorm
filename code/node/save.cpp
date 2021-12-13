struct SaveFile {
	FILE *file;
};

template <typename T>
void SaveVMArray(VMArray<T> array, SaveFile *saveFile)
{
	// save vertices
	int count = array.Count();
	int max = array.Max();

	fwrite(&count, sizeof(int), 1, saveFile->file);
	fwrite(&max, sizeof(int), 1, saveFile->file);
	fwrite(array.Data(), sizeof(T) * count, 1, saveFile->file);
}


/*
  This function assumes that the next piece of data in the file
  is a VMArray of type T
 */
template <typename T>
VMArray<T> LoadNextVMArray(SaveFile *saveFile)
{
	int count;
	int max;
	fread(&count, sizeof(int), 1, saveFile->file);
	fread(&max, sizeof(int), 1, saveFile->file);
	// TODO (rhoe) maybe its possibly to avoid this extra malloc
	T *data = (T*)malloc(sizeof(T) * count);
	fread(data, sizeof(T) * count, 1, saveFile->file);
	VMArray<T> result = VMArray<T>(max, count, data);
	free(data);
	return result;
}

void SaveInt(int i, SaveFile *saveFile)
{
	fwrite(&i, sizeof(int), 1, saveFile->file);
}

void SaveNodes()
{
	//////////////
	// SAVE NODES
	SaveFile saveFile = {};
	saveFile.file = fopen("testsave.octo", "wb");
	SaveVMArray<Node>(_nodeState->nodes, &saveFile);

	//////////////
	// SAVE DATA
	SaveInt(_nodeState->textures.Count(), &saveFile);
	SaveInt(_nodeState->meshes.Count(), &saveFile);
	SaveInt(_nodeState->renderObjects.Count(), &saveFile);
	SaveInt(_nodeState->doubles.Count(), &saveFile);
	SaveInt(_nodeState->videoNodes.Count(), &saveFile);


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

	_nodeState->nodes = LoadNextVMArray<Node>(&saveFile);

	// mark all nodes as dirty after load so we can regenerate all resources
	for(int i = 0; i < _nodeState->nodes.Count(); i++) {
		_nodeState->nodes[i].changed = true;
		_nodeState->nodes[i].initialized = false;
	}

	// load textures
	{
		_nodeState->textures.Clear();
		int count;
		fread(&count, sizeof(int), 1, saveFile.file);
		for(int i = 0; i < count; i++) {
			_nodeState->textures.InsertNew();
		}
	}

	// load meshes
	{
		_nodeState->meshes.Clear();
		int count;
		fread(&count, sizeof(int), 1, saveFile.file);
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

		int count;
		fread(&count, sizeof(int), 1, saveFile.file);
		for(int i = 0; i < count; i++) {
			AddNewRenderObject();
		}
	}

	// load doubles
	{
		_nodeState->doubles.Clear();
		int count;
		fread(&count, sizeof(int), 1, saveFile.file);
		for(int i = 0; i < count; i++) {
			double value = 0.0;
			_nodeState->doubles.Insert(value);
		}
	}

	// load video nodes
	{
		_nodeState->videoNodes.Clear();
		int count;
		fread(&count, sizeof(int), 1, saveFile.file);
		for(int i = 0; i < count; i++) {
			_nodeState->videoNodes.Insert(VideoNodeState());
		}
	}
}
