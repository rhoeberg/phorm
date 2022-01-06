#include "node_parameter.h"

NodeParameter::NodeParameter(const char *_name, int _i)
{
	type = DATA_INT;
	sprintf(name, "%s", _name);
	i = _i;
	nodeHandle.isset = false;
	exposed = true;
}

NodeParameter::NodeParameter(const char *_name, double _d)
{
	type = DATA_DOUBLE;
	sprintf(name, "%s", _name);
	d = _d;
	nodeHandle.isset = false;
	exposed = true;
}

NodeParameter::NodeParameter(const char *_name, vec3 _v3)
{
	type = DATA_VEC3;
	sprintf(name, "%s", _name);
	v3 = _v3;
	nodeHandle.isset = false;
	exposed = false;
}

NodeParameter::NodeParameter(const char *_name, char *str)
{
	dataHandle = AddString(str);
	sprintf(name, "%s", _name);
	type = DATA_STRING;
	nodeHandle.isset = false;
	exposed = false;
}

NodeParameter::NodeParameter(const char *_name, ObjectHandle handle)
{
	// type = handle.type;
	sprintf(name, "%s", _name);
	dataHandle = handle;
	nodeHandle.isset = false;
	exposed = false;
}

// int NodeParameter::Int()
// {
// 	if(nodeHandle.isset) {
// 		// double *doubleFromNode = GetDoubleOutput(&nodeHandle);
// 		Node *node = GetNode(&nodeHandle);
// 		double *doubleFromNode = GetInts()->Get(&node->GetData());
// 		if(doubleFromNode) {
// 			return *doubleFromNode;
// 		}
// 		else {
// 			return 0.0;
// 		}
// 	}

// 	if(type == DATA_INT)
// 		return i;

// 	return 0;
// }

double NodeParameter::Double()
{
	if(nodeHandle.isset) {
		// double *doubleFromNode = GetDoubleOutput(&nodeHandle);
		Node *node = GetNode(&nodeHandle);
		double *doubleFromNode = GetDoubles()->Get(&node->GetData());
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
			vec3 *output = GetVec3s()->Get(&node->GetData());
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

vec4 NodeParameter::Vec4()
{
	if(nodeHandle.isset) {
		Node *node = GetNode(&nodeHandle);
		if(node) {
			vec4 *output = GetVec4s()->Get(&node->GetData());
			if(output) {
				return *output;
			}
			else {
				return vec4(0, 0, 0, 0);
			}
		}
	}

	if(type == DATA_VEC4)
		return v4;

	return vec4(0, 0, 0, 0);
}
