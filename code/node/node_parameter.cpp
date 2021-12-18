#include "node_parameter.h"

NodeParameter::NodeParameter(const char *_name, int _i)
{
	type = DATA_INT;
	sprintf(name, "%s", _name);
	i = _i;
	nodeHandle.isset = false;
}

NodeParameter::NodeParameter(const char *_name, double _d)
{
	type = DATA_DOUBLE;
	sprintf(name, "%s", _name);
	d = _d;
	nodeHandle.isset = false;
}

NodeParameter::NodeParameter(const char *_name, vec3 _v3)
{
	type = DATA_VEC3;
	sprintf(name, "%s", _name);
	v3 = _v3;
	nodeHandle.isset = false;
}

NodeParameter::NodeParameter(const char *_name, char *str)
{
	dataHandle = AddString(str);
	sprintf(name, "%s", _name);
	type = DATA_STRING;
	nodeHandle.isset = false;
}

NodeParameter::NodeParameter(const char *_name, ObjectHandle handle)
{
	// type = handle.type;
	sprintf(name, "%s", _name);
	dataHandle = handle;
	nodeHandle.isset = false;
}

double NodeParameter::Double()
{
	if(nodeHandle.isset) {
		double *doubleFromNode = GetDoubleOutput(&nodeHandle);
		if(doubleFromNode) {
			return *doubleFromNode;
		}
		else {
			return 0.0;
		}
	}

	if(type == DATA_DOUBLE)
		return d;

	return 0.0;
}

vec3 NodeParameter::Vec3()
{
	if(nodeHandle.isset) {
		Node *node = GetNode(&nodeHandle);
		if(node) {
			vec3 *output = GetVec3(&node->GetData());
			if(output) {
				return *output;
			}
			else {
				return vec3(0, 0, 0);
			}
		}
	}

	if(type == DATA_VEC3)
		return v3;

	return vec3(0, 0, 0);
}
