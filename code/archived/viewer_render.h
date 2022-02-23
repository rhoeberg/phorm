#pragma once
#include "Camera.h"

#define VIEWER_SIZE 600

struct RenderObjectInstance {
	ObjectHandle renderObjectHandle;
	vec3 pos;
	vec3 scale;
	vec3 rot;
	vec4 color;

	RenderObjectInstance() {}

	RenderObjectInstance(ObjectHandle _renderObjectHandle)
	{
		renderObjectHandle = _renderObjectHandle;
		pos = vec3(0, 0, 0);
		scale = vec3(1, 1, 1);
		rot = vec3(0, 0, 0);
		color = vec4(1, 1, 1, 1);
	}

	RenderObjectInstance(ObjectHandle _renderObjectHandle, vec3 _pos, vec3 _scale, vec3 _rot, vec4 _color)
	{
		renderObjectHandle = _renderObjectHandle;
		pos = _pos;
		scale = _scale;
		rot = _rot;
		color = _color;
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

struct RenderLightInstance {
	vec3 pos;
	vec3 color;

	RenderLightInstance(){}

	RenderLightInstance(vec3 _pos, vec4 _color)
	{
		pos = _pos;
		color = _color;
	}
};

struct TransformGizmoState {
	ObjectHandle handle;
	bool isDragging;
	vec3 dragStart;
	vec3 dragPlaneNormal;
	vec3 dragAxisLock;
};

struct ViewerRenderState {
	VMArray<RenderObjectInstance> renderList;
	VMArray<RenderGroupInstance> renderGroupList;
 	VMArray<RenderLightInstance> renderPointLights;
	ObjectHandle baseTextureObject;
	bool wireframe;

	GLuint defaultTexture;
	Shader defaultShader;

	/////////////
	// FRAMEBUFFER FOR MAIN WINDOW VIEWER
	GLuint fbo;
	GLuint fboTexture;
	GLuint quad;

	/////////////
	// CAMERA
	Camera cam;

	bool dragging;
	vec2 startDragPos;
	vec2 lastDragPos;

	/////////////
	// GIZMOS
	TransformGizmoState transformGizmo;
};

void InitializeViewerRender();
bool MouseInsideViewerRect();
void UpdateViewerRender();
void AddToRenderQueue(RenderObjectInstance instance);
void AddToRenderGroupQueue(RenderGroupInstance instance);
void AddTextureToRenderQueue(ObjectHandle handle);
void AddToRenderPointLightQueue(RenderLightInstance instance);
void UpdateGizmos();

global ViewerRenderState _viewerRenderState;