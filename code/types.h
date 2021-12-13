#pragma once

enum NodeDataType {
	DATA_TEXTURE,
	DATA_MESH,
	DATA_RENDEROBJECT,
	DATA_DOUBLE,
	DATA_INT,
	DATA_VEC3,
	DATA_STRING,
	DATA_NONE,
};

struct DataHandle {
	unsigned int id;
	NodeDataType type;
	/* int slotID; */
};

struct NodeHandle {
	unsigned int id;
	NodeDataType type;
};
