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

	ObjectContainer()
	{
		elements = VMArray<T>();
		isFree = VMArray<bool>();
		slotID = VMArray<u32>();
	}

	NodeHandle Insert(T e)
	{
		NodeHandle result = {};
		// check for free slot reuse
		for(int i = 0; i < isFree.Count(); i++) {
			if(isFree[i]) {
				elements[i] = e;
				isFree[i] = false;
				slotID[i]++;

				result.id = i;
				result.slotID = slotID[i];
				return result;
			}
		}

		// insert new slot
		result.id = elements.Insert(e);
		result.slotID = 0;
		isFree.Insert(false);
		slotID.Insert(result.slotID);

		return result;
	}

	// we simply set the array slot to free when removing
	// insert takes care of checking for empty slots to use instead of inserting new
	void Remove(NodeHandle handle)
	{
		// make sure handle exist to avoid removing
		// wrong slotID or handle with different type
		if(!Exists(handle)) {
			return;
		}

		isFree[handle.id] = true;
	}

	bool Exists(NodeHandle handle)
	{
		// out of bounds
		if(handle.id > (elements.Count() - 1) ||
		   handle.id < 0) {
			return false;
		}

		// slot is empty
		if(isFree[handle.id]) {
			return false;
		}

		// wrong slot id
		if(slotID[handle.id] != handle.slotID) {
			return false;
		}

		return true;
	}

	T* Get(NodeHandle handle)
	{
		if(!Exists(handle)) {
			return NULL;
		}
		// return &elements[handle.id];
		return dynamic_cast<T*>(&elements[handle.id]);
	}

	// Used for iterating with requesting a specific handl
	T* GetAt(int index)
	{
		// out of bounds
		if(handle.id > (elements.Count() - 1) ||
		   handle.id < 0) {
			return NULL;
		}

		return &elements[index];
	}

	NodeHandle GetHandle(int index)
	{
		NodeHandle handle = {};
		handle.id = index;
		handle.slotID = slotID[index];
		return handle;
	}

	int Count()
	{
		return elements.Count();
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
	VMArray<Texture> textures;
	VMArray<Mesh> meshes;
	VMArray<RenderObject> renderObjects;
	VMArray<double> doubles;
	HashMap<NodeHandle> labels;
	VMArray<VideoNodeState> videoNodes;
	VMArray<LabelNodeState> labelNodes;
	VMArray<String> strings;
};

global NodeState *_nodeState;

bool NodeExists(NodeHandle handle);
Node* GetNode(NodeHandle handle);
double* GetDouble(DataHandle handle);
double* GetDoubleOutput(NodeHandle handle);
Texture* GetTexture(DataHandle handle);
Texture* GetTextureInput(NodeInput input);
RenderObject* GetRenderObject(DataHandle handle);
Mesh* GetMesh(DataHandle handle);
Mesh* GetMeshInput(NodeInput input);
bool ConnectNodeParameter(NodeHandle handle, NodeHandle outHandle, int paramIndex);
bool ConnectNodeInput(NodeHandle inHandle, NodeHandle outHandle, int inputIndex);
RenderObject CreateRenderObject();
DataHandle AddNewRenderObject();
DataHandle AddString(char *value);
String* GetString(DataHandle handle);


// TODO (rhoe) function that gets data resource in a generic way
/* template <typename T> */
/* T GetData<T>(DataHandle handle) */
// needs specialized template implementations
// template<typename T>
// T GetData<T>(DataHandle handle)
