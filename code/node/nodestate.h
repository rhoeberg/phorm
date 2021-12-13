#pragma once

struct VideoNodeState {
	plm_t *plm;
};

struct LabelNodeState {
	String key;
};

// NEXT deletion of nodes
// we will use a dataslot type to verify our slots
template <typename T>
struct DataSlot {
	T t;
	int slotID; 
	bool free;
};

struct NodeState {
	// base node array
	VMArray<Node> nodes;

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
