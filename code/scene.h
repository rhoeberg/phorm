#pragma once

/* struct SceneNode { */
/* 	String labelKey; */
/* 	NodeHandle renderNode; */
/* }; */

struct Scene {
	/* VMArray<String> keys; */
	/* VMArray<ObjectHandle> objects; */
	ObjectContainer<ObjectHandle> objects;

	Scene() {
		/* objects = VMArray<ObjectHandle>(); */
		/* objects = ObjectContainer<ObjectHandle>(HANDLE_HANDLE); */
	}

	void Free() {
		objects.Free();
	}
};
