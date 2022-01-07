#pragma once

struct NodeParameter {
	DataType type;
	ObjectHandle nodeHandle;
	char name[128];
	bool exposed;

	union {
		int i;
		double d;
		vec3 v3;
		vec4 v4;

		// TOOD (rhoe) here we could use a handle to a String type stored somewhere else
		/* char str[128]; */
		ObjectHandle dataHandle;
	};

	NodeParameter() {};
	NodeParameter(const char *_name, i32 _i);
	NodeParameter(const char *_name, double _d);
	NodeParameter(const char *_name, vec3 _v3);
	NodeParameter(const char *_name, const char *str);
	NodeParameter(const char *_name, ObjectHandle handle);
	double Double();
	vec3 Vec3();
	vec4 Vec4();
};
