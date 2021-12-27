#pragma once

#define VIEWER_SIZE 600

struct RenderObjectInstance {
	ObjectHandle renderObjectHandle;
	vec3 pos;
	vec3 scale;
	vec3 rot;

	RenderObjectInstance() {}

	RenderObjectInstance(ObjectHandle _renderObjectHandle)
	{
		renderObjectHandle = _renderObjectHandle;
		pos = vec3(0, 0, 0);
		scale = vec3(1, 1, 1);
		rot = vec3(0, 0, 0);
	}

	RenderObjectInstance(ObjectHandle _renderObjectHandle, vec3 _pos, vec3 _scale, vec3 _rot)
	{
		renderObjectHandle = _renderObjectHandle;
		pos = _pos;
		scale = _scale;
		rot = _rot;
	}
};

struct RenderGroupInstance {
	ObjectHandle renderGroupHandle;
	vec3 pos;
	vec3 scale;
	vec3 rot;

	RenderGroupInstance(ObjectHandle _renderGroupHandle, vec3 _pos, vec3 _scale, vec3 _rot)
	{
		renderGroupHandle = _renderGroupHandle;
		pos = _pos;
		scale = _scale;
		rot = _rot;
	}
};

struct ViewerRenderState {
	VMArray<RenderObjectInstance> renderList;
	VMArray<RenderGroupInstance> renderGroupList;
	VMArray<ObjectHandle> renderPointLights;
	ObjectHandle baseTextureObject;
	bool wireframe;

	GLuint defaultTexture;
	GLuint defaultShader;

	/////////////
	// FRAMEBUFFER FOR MAIN WINDOW VIEWER
	GLuint fbo;
	GLuint fboTexture;
	GLuint quad;

	/////////////
	// CAMERA
	bool dragging;
	vec2 startDragPos;
	vec2 lastDragPos;
	vec2 dragAmount;
	float orbitDistance;
	float orbitDragSpeed;
};

void InitializeViewerRender();
void UpdateViewerRender();
void AddToRenderQueue(RenderObjectInstance instance);
void AddToRenderGroupQueue(RenderGroupInstance instance);
void AddTextureToRenderQueue(ObjectHandle *textureHandle);
void AddToRenderPointLightQueue(ObjectHandle *handle);

global ViewerRenderState _viewerRenderState;
