#pragma once

/* struct SceneNode { */
/* 	String labelKey; */
/* 	NodeHandle renderNode; */
/* }; */

struct Scene {
	/* VMArray<String> keys; */
	/* VMArray<ObjectHandle> objects; */
	ObjectContainer<ObjectHandle> objects;
	ObjectContainer<ObjectHandle> pointLights;

	Scene() {
		/* objects = VMArray<ObjectHandle>(); */
		objects = ObjectContainer<ObjectHandle>(HANDLE_OBJECTHANDLE, DATA_RENDEROBJECT);
		pointLights = ObjectContainer<ObjectHandle>(HANDLE_OBJECTHANDLE, DATA_POINTLIGHT);
	}

	void Free() {
		objects.Free();
	}
};
