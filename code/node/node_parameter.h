#pragma once

struct NodeParameter {
	DataType type;
	NodeHandle nodeHandle;
	bool handleIsset;
	char name[128];
	bool exposed;

	union {
		int i;
		double d;
		vec3 v3;

		// TOOD (rhoe) here we could use a handle to a String type stored somewhere else
		/* char str[128]; */
		DataHandle dataHandle;
	};

	NodeParameter() {};
	NodeParameter(const char *_name, int _i);
	NodeParameter(const char *_name, double _d);
	NodeParameter(const char *_name, vec3 _v3);
	NodeParameter(const char *_name, char *str);
	NodeParameter(const char *_name, DataHandle handle);
	double Double();
};
