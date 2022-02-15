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
	ObjectContainer<Scene> scenes;

	ObjectContainer<SceneRenderData> sceneRenderDatas;
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
ObjectContainer<Scene>* GetScenes();
ObjectContainer<SceneRenderData>* GetSceneRenderDatas();

RenderObject CreateRenderObject();
ObjectHandle AddNewRenderObject();
ObjectHandle AddString(const char *value);
ObjectHandle AddNewTexture(u32 width, u32 height);
void CleanupData();
