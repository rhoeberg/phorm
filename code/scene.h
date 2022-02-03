#pragma once
/* #include "viewer_render.h" */
/* #include "ViewerRender.h" */
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

struct SceneObject
{
	ObjectHandle handle; // handle to node (can be renderobject or renderobjectgroup)
	vec3 pos;
	vec3 scale;
	vec3 rot;
	vec4 color;

	SceneObject(ObjectHandle _handle)
	{
		handle = _handle;
		pos = vec3(0, 0, 0);
		scale = vec3(1, 1, 1);
		rot = vec3(0, 0, 0);
		color = vec4(1, 1, 1, 1);
	}

	RenderObjectInstance ToRenderObjectInstance(ObjectHandle _renderObjectHandle)
	{
		RenderObjectInstance result = {};
		result.renderObjectHandle = _renderObjectHandle;
		result.pos = pos;
		result.scale = scale;
		result.rot = rot;
		result.color = color;

		return result;
	}
};

struct Scene {
	vec3 bgColor;
	ObjectContainer<SceneObject> sceneObjects;
	ObjectContainer<ObjectHandle> pointLights;

	void Free() {
		sceneObjects.Free();
		/* pointLights.Free(); */
	}
};

void RenderScene2(ObjectHandle sceneHandle, ObjectHandle textureHandle);
