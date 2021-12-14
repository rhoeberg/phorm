#pragma once

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

enum DataType {
	DATA_TEXTURE,
	DATA_MESH,
	DATA_RENDEROBJECT,
	DATA_DOUBLE,
	DATA_INT,
	DATA_VEC3,
	DATA_STRING,
	DATA_VIDEO_STATE,
	DATA_NONE,
};

enum HandleType {
	HANDLE_NONE,
	HANDLE_NODE,
	HANDLE_DATA,
	HANDLE_OBJECTHANDLE,
};

struct ObjectHandle {
	unsigned int id;
	u32 slotID;
	HandleType handleType;
	DataType dataType; // only used for data handles
	bool isset;

	ObjectHandle() {
		isset = false;
	}
};
