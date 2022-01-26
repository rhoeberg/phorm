#pragma once

#include "light.h"
#include "texture.h"
#include "mesh.h"
#include "renderobject.h"
#include "RenderObjectGroup.h"

struct VideoNodeState {
	plm_t *plm;
};

struct LabelNodeState {
	String key; // add this to strings so we can serialize easily
};

struct SinWaveNodeState {
	float histogram[2000];
	i32 last;
};

struct TabelNodeState {
	i32 values[32];
};

struct NodeState {
	// base node array
	// VMArray<Node> nodes;
	ObjectContainer<Node> nodes;

	// data arrays
	ObjectContainer<Texture> textures;
	ObjectContainer<Mesh> meshes;
	ObjectContainer<RenderObject> renderObjects;
	ObjectContainer<RenderObjectGroup> renderObjectGroups;
	ObjectContainer<double> doubles;
	ObjectContainer<int> ints;
	ObjectContainer<vec3> vec3s;
	ObjectContainer<vec4> vec4s;
	// HashMap<ObjectHandle> labels;
	// ObjectContainer<LabelNodeState> labelNodes;
	ObjectContainer<String> strings;
	ObjectContainer<PointLight> pointLights;

	ObjectContainer<VideoNodeState> videoNodes;
	ObjectContainer<SinWaveNodeState> sinWaveNodes;
};

global NodeState *_nodeState;

ObjectContainer<Node>* GetNodes();
ObjectContainer<Texture>* GetTextures();
ObjectContainer<Mesh>* GetMeshes();
ObjectContainer<double>* GetDoubles();
ObjectContainer<int>* GetInts();
ObjectContainer<RenderObject>* GetRenderObjects();
ObjectContainer<RenderObjectGroup>* GetRenderObjectGroups();
ObjectContainer<vec3>* GetVec3s();
ObjectContainer<vec4>* GetVec4s();
ObjectContainer<PointLight>* GetPointLights();
ObjectContainer<String>* GetStrings();

RenderObject CreateRenderObject();
ObjectHandle AddNewRenderObject();
ObjectHandle AddString(const char *value);
