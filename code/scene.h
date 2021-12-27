#pragma once
#include "viewer_render.h"

struct SceneObject
{
	ObjectHandle handle; // handle to node (can be renderobject or renderobjectgroup)
	vec3 pos;
	vec3 scale;
	vec3 rot;

	SceneObject(ObjectHandle _handle)
	{
		handle = _handle;
		pos = vec3(0, 0, 0);
		scale = vec3(1, 1, 1);
		rot = vec3(0, 0, 0);
	}
};

struct Scene {
	ObjectContainer<SceneObject> sceneObjects;
	ObjectContainer<ObjectHandle> pointLights;

	Scene() {
		sceneObjects = ObjectContainer<SceneObject>();
		pointLights = ObjectContainer<ObjectHandle>(HANDLE_OBJECTHANDLE, DATA_POINTLIGHT);
	}

	void Free() {
		sceneObjects.Free();
		pointLights.Free();
	}
};
