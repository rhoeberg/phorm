#pragma once

#include "light.h"
#include "Bitmap.h"
#include "mesh.h"
#include "PhormRenderObject.h"
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

struct MeshComputeState
{
	Shader compute;
	u32 buffer; 
	i32 size; 
};

struct BitmapComputeState
{
	Shader compute;
	u32 textureHandle;
	u32 pboHandle;
	/* u32 bufferHandle; */
	i32 width;
	i32 height;
};

// TODO (rhoe) change this name to something better
struct SceneRenderData
{
	GLuint fbo;
	GLuint pbo;
	GLuint rbo;
	GLuint textureID;

	i32 width;
	i32 height;

	SceneRenderData() {
		SetContextViewer();
		glGenFramebuffers(1, &fbo);
		glGenRenderbuffers(1, &rbo);
		glGenTextures(1, &textureID);
		glGenBuffers(1, &pbo);

		SetSize(800, 600);
	}

	void SetSize(i32 _width, i32 _height)
	{
		if(width != _width || height != _height) {
			width = _width;
			height = _height;
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

			// create RBO
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

			if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				ErrorLog("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
			glBindFramebuffer(GL_FRAMEBUFFER, 0);  

			// Create PBO
			glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo);
			glBufferData(GL_PIXEL_PACK_BUFFER, width * height * sizeof(Pixel), NULL, GL_STREAM_COPY);
		}
	}

};

/*
  For a easier way to add custom data to nodes instead of having to
  add a new custom struct to data.h and then add the container to the 
  nodestate etc..
  We could have a super data struct which contains all objectcontainers 
  (similar to our nodestate struct now)
  but instead of hardcoding the different data types into it, we would
  have a way to insert an arbitrary objectcontainer into it.

  this could be done by having some sort of list of objectcontainers.

  how would we then access and get a specific objectcontainer?
  if we could somehow restrict container insertion to one container pr. type
  then maybe we could use the type to retrieve the container.
  alternativly we need to keep track of some indices
  
  problems:
  im not even sure this is possible in c++ since we would need a list of generic lists
  and im not sure templates can take care of that.
  another problem is retrieving the data, its hard to do it in a clean way and it
  will probably add some indirection would could potentialy be a performance problem,
  considering how often we need to retrieve data.

  
  instead of templates its probably better to use pointers in some way.
  still an indirection but might be doable.

  
  
*/

struct NodeState {
	// NODES
	ObjectContainer<Node> nodes;

	// DATA
	ObjectContainer<Bitmap> bitmaps;
	ObjectContainer<Mesh> meshes;
	ObjectContainer<RenderObject> renderObjects;
	ObjectContainer<RenderObjectGroup> renderObjectGroups;
	ObjectContainer<double> doubles;
	ObjectContainer<int> ints;
	ObjectContainer<vec3> vec3s;
	ObjectContainer<vec4> vec4s;
	ObjectContainer<String> strings;
	ObjectContainer<PointLight> pointLights;
	ObjectContainer<Scene> scenes;
	ObjectContainer<Camera> cameras;

	// NODE SPECIFIC STATE
	ObjectContainer<SceneRenderData> sceneRenderDatas;
	ObjectContainer<VideoNodeState> videoNodes;
	ObjectContainer<SinWaveNodeState> sinWaveNodes;
	ObjectContainer<BitmapComputeState> bitmapComputeStates;
	ObjectContainer<MeshComputeState> meshComputeStates;
	/* ObjectContainer<ParticleNodeState> particleNodeStates; */
};

global NodeState *_nodeState;

ObjectContainer<Node>* GetNodes();
ObjectContainer<Bitmap>* GetBitmaps();
ObjectContainer<Mesh>* GetMeshes();
ObjectContainer<double>* GetDoubles();
ObjectContainer<int>* GetInts();
ObjectContainer<RenderObject>* GetRenderObjects();
ObjectContainer<RenderObjectGroup>* GetRenderObjectGroups();
ObjectContainer<vec3>* GetVec3s();
ObjectContainer<vec4>* GetVec4s();
ObjectContainer<PointLight>* GetPointLights();
ObjectContainer<String>* GetStrings();
ObjectContainer<Scene>* GetScenes();
ObjectContainer<Camera>* GetCameras();
ObjectContainer<SceneRenderData>* GetSceneRenderDatas();
ObjectContainer<BitmapComputeState>* GetBitmapComputeStates();
ObjectContainer<MeshComputeState>* GetMeshComputeStates();
/* ObjectContainer<ParticleNodeState>* GetParticleNodeStates(); */

RenderObject CreateRenderObject();
ObjectHandle AddNewRenderObject();
ObjectHandle AddString(const char *value);
ObjectHandle AddNewTexture(u32 width, u32 height);
void CleanupData();
