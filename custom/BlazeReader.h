#pragma once

struct PythonData {
	int len;
	vec3 data[33];
};

struct BlazeReaderState
{
	PythonData pData;
};

// TODO (rhoe) allocate this somewhere
global BlazeReaderState _blazeReaderState;

enum BodyPartIndex {
	NOSE = 0,
	LEFT_EYE_INNER = 1,
	LEFT_EYE = 2,
	LEFT_EYE_OUTER = 3,
	RIGHT_EYE_INNER = 4,
	RIGHT_EYE = 5,
	RIGHT_EYE_OUTER = 6,
	LEFT_EAR = 7,
	RIGHT_EAR = 8,
	MOUTH_LEFT = 9,
	MOUTH_RIGHT = 10,
	LEFT_SHOULDER = 11,
	RIGHT_SHOULDER = 12,
	LEFT_ELBOW = 13,
	RIGHT_ELBOW = 14,
	LEFT_WRIST = 15,
	RIGHT_WRIST = 16,
	LEFT_PINKY = 17,
	RIGHT_PINKY = 18,
	LEFT_INDEX = 19,
	RIGHT_INDEX = 20,
	LEFT_THUMB = 21,
	RIGHT_THUMB = 22,
	LEFT_HIP = 23,
	RIGHT_HIP = 24,
	LEFT_KNEE = 25,
	RIGHT_KNEE = 26,
	LEFT_ANKLE = 27,
	RIGHT_ANKLE = 28,
	LEFT_HEEL = 29,
	RIGHT_HEEL = 30,
	LEFT_FOOT_INDEX = 31,
	RIGHT_FOOT_INDEX = 32,
};

void BlazeReaderOp(Node *self)
{
	/* if(UDPNewMessage()) { */
		/* NetworkMessage msg = GetLastMessage(); */
		/* if(msg.length > 0) { */

		/* } */
	/* } */

	i32 test = 0;
	i32 len = UDPRecieve((char*)&_blazeReaderState.pData, sizeof(PythonData));
	if(len > 0) {
		test = 1;
	}
}

ObjectHandle CreateBlazeReaderNode()
{
	FixedArray<NodeParameter> params = {
	};

	FixedArray<NodeInput> inputs = {
	};

	ObjectHandle result = AddNode(DATA_NONE, params, inputs);
	Node *node = GetNode(result);
	if(node) {
		node->update = true;
	}

	return result;
}
