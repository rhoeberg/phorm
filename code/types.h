#pragma once

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

typedef glm::vec4 vec4;
typedef glm::vec3 vec3;
typedef glm::vec2 vec2;
typedef glm::quat quat;

enum DataType {
	DATA_TEXTURE,
	DATA_MESH,
	DATA_RENDEROBJECT,
	DATA_RENDEROBJECT_GROUP,
	DATA_DOUBLE,
	DATA_INT,
	DATA_VEC3,
	DATA_STRING,
	DATA_VIDEO_STATE,
	DATA_POINTLIGHT,
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

	// TODO (rhoe) this is not testet
	bool operator==(const ObjectHandle &other) {
		if(slotID == other.slotID &&
		   id == other.id &&
		   handleType == other.handleType &&
		   dataType == other.dataType)
			return true;

		return false;
	}
};

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define ASSERT(Expression) if(!(Expression)) {*(volatile int *)0 = 0;}
#define NOT_IMPLEMENTED ASSERT(!"NOT_IMPLEMENTED")
