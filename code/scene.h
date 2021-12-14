#pragma once

/* struct SceneNode { */
/* 	String labelKey; */
/* 	NodeHandle renderNode; */
/* }; */

struct Scene {
	/* VMArray<String> keys; */
	VMArray<ObjectHandle> objects;

	Scene() {
		objects = VMArray<ObjectHandle>();
	}

	void Free() {
		objects.Free();
	}
};
