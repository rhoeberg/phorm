#pragma once

struct VideoNodeState {
	plm_t *plm;
};

struct LabelNodeState {
	String key; // add this to strings so we can serialize easily
};

// NEXT deletion of nodes
// we will use a dataslot type to verify our slots
// template <typename T>
// struct DataSlot {
// 	T t;
// 	int slotIndex; 
// 	bool free;
// };

template <typename T>
struct ObjectContainer {

	// its very important that these 3 arrays are completely synced.
	// each index in the arrays refers to the same objects
	VMArray<T> elements;
	VMArray<bool> isFree;
	VMArray<u32> slotID;

	// TODO put back type here
	HandleType handleType;
	DataType dataType;

	ObjectContainer()
	{
		handleType = HANDLE_NONE;
	}

	ObjectContainer(HandleType _handleType, DataType _dataType = DATA_NONE)
	{
		elements = VMArray<T>();
		isFree = VMArray<bool>();
		slotID = VMArray<u32>();

		handleType = _handleType;
		dataType = _dataType;
	}

	// Used for serialization
	ObjectHandle InsertRaw(T e, bool _free, u32 _slotID)
	{
		ObjectHandle result = {};
		result.id = elements.Insert(e);
		result.slotID = _slotID;
		isFree.Insert(_free);
		slotID.Insert(_slotID);
		result.handleType = handleType;
		result.dataType = dataType;
		return result;
	}

	ObjectHandle Insert(T e)
	{
		ObjectHandle result = {};
		// check for free slot reuse
		for(int i = 0; i < isFree.Count(); i++) {
			if(isFree[i]) {
				elements[i] = e;
				isFree[i] = false;
				slotID[i]++;

				result.id = i;
				result.slotID = slotID[i];
				result.handleType = handleType;
				result.dataType = dataType;
				return result;
			}
		}

		// insert new slot
		result.id = elements.Insert(e);
		result.slotID = 0;
		isFree.Insert(false);
		slotID.Insert(result.slotID);
		result.handleType = handleType;
		result.dataType = dataType;

		return result;
	}

	ObjectHandle InsertNew()
	{
		ObjectHandle result = {};
		// check for free slot reuse
		for(int i = 0; i < isFree.Count(); i++) {
			if(isFree[i]) {
				isFree[i] = false;
				slotID[i]++;

				result.id = i;
				result.slotID = slotID[i];
				result.handleType = handleType;
				result.dataType = dataType;
				return result;
			}
		}

		// insert new slot
		result.id = elements.InsertNew();
		result.slotID = 0;
		isFree.Insert(false);
		slotID.Insert(result.slotID);
		result.handleType = handleType;
		result.dataType = dataType;

		return result;
	}

	// we simply set the array slot to free when removing
	// insert takes care of checking for empty slots to use instead of inserting new
	void Remove(ObjectHandle *handle)
	{
		// make sure handle exist to avoid removing
		// wrong slotID or handle with different type
		if(!Exists(handle)) {
			return;
		}

		isFree[handle->id] = true;
	}

	bool Exists(ObjectHandle *handle)
	{
		// wrong type
		if(handle->handleType != handleType ||
		   handle->dataType != dataType) {
			return false;
		}

		// out of bounds
		if(handle->id > (elements.Count() - 1) ||
		   handle->id < 0) {
			return false;
		}

		// slot is empty
		if(isFree[handle->id]) {
			return false;
		}

		// wrong slot id
		if(slotID[handle->id] != handle->slotID) {
			return false;
		}

		return true;
	}

	T* Get(ObjectHandle *handle)
	{
		if(!Exists(handle)) {
			handle->isset = false;
			return NULL;
		}

		// TODO (rhoe) make sure we have some kind of type validation here
		return &elements[handle->id];
	}

	// // Used for iterating with requesting a specific handle
	// T* GetAt(int index)
	// {
	// 	// out of bounds
	// 	if(handle.id > (elements.Count() - 1) ||
	// 	   handle.id < 0) {
	// 		return NULL;
	// 	}

	// 	return &elements[index];
	// }

	ObjectHandle GetHandle(int index)
	{
		ObjectHandle handle = {};
		handle.id = index;
		handle.slotID = slotID[index];
		handle.handleType = handleType;
		handle.dataType = dataType;

		if(Exists(&handle)) {
			handle.isset = true;
		}

		return handle;
	}

	int Count()
	{
		return elements.Count();
	}

	void Clear()
	{
		elements.Clear();
		isFree.Clear();
		slotID.Clear();
	}

	void Free()
	{
		elements.Free();
		isFree.Free();
		slotID.Free();
	}
};

struct NodeState {
	// base node array
	// VMArray<Node> nodes;
	ObjectContainer<Node> nodes;

	// data arrays
	ObjectContainer<Texture> textures;
	ObjectContainer<Mesh> meshes;
	ObjectContainer<RenderObject> renderObjects;
	ObjectContainer<double> doubles;
	// HashMap<ObjectHandle> labels;
	ObjectContainer<VideoNodeState> videoNodes;
	// ObjectContainer<LabelNodeState> labelNodes;
	ObjectContainer<String> strings;
};

global NodeState *_nodeState;

bool NodeExists(ObjectHandle *handle);
Node* GetNode(ObjectHandle *handle);
void DeleteNode(ObjectHandle *handle);
double* GetDouble(ObjectHandle *handle);
double* GetDoubleOutput(ObjectHandle *handle);
Texture* GetTexture(ObjectHandle *handle);
Texture* GetTextureInput(NodeInput input);
RenderObject* GetRenderObject(ObjectHandle *handle);
Mesh* GetMesh(ObjectHandle *handle);
Mesh* GetMeshInput(NodeInput input);
bool ConnectNodeParameter(ObjectHandle *handle, ObjectHandle *outHandle, int paramIndex);
bool ConnectNodeInput(ObjectHandle *inHandle, ObjectHandle *outHandle, int inputIndex);
RenderObject CreateRenderObject();
ObjectHandle AddNewRenderObject();
ObjectHandle AddString(char *value);
String* GetString(ObjectHandle *handle);


// TODO (rhoe) function that gets data resource in a generic way
/* template <typename T> */
/* T GetData<T>(DataHandle handle) */
// needs specialized template implementations
// template<typename T>
// T GetData<T>(DataHandle handle)
