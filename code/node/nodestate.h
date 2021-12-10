#pragma once

struct VideoNodeState {
	plm_t *plm;
};

struct NodeState {
	// base node array
	VMArray<Node> nodes;

	// data arrays
	VMArray<Texture> textures;
	VMArray<Mesh> meshes;
	VMArray<RenderObject> renderObjects;
	VMArray<double> doubles;
	VMArray<VideoNodeState> videoNodes;
	HashMap<NodeHandle> labels;
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
int AddNode(const char *name, NodeDataType type, NodeOp op, FixedArray<NodeParameter> params, FixedArray<NodeInput> inputs);
/* int AddNode(const char *name, NodeDataType type, NodeOp op, NodeDrawFunc drawFunc, VMArray<NodeParameter> params, VMArray<NodeInput> inputs); */
