#pragma once

/* struct SceneNode { */
/* 	String labelKey; */
/* 	NodeHandle renderNode; */
/* }; */

struct Scene {
	/* VMArray<String> keys; */
	VMArray<NodeHandle> objects;

	Scene() {
		objects = VMArray<NodeHandle>();
	}

	void Free() {
		objects.Free();
	}
};
