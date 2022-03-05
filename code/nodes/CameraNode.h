#pragma once

void CameraNodeOp(Node *self)
{
	Camera *output = GetCameras()->Get(self->GetDataLast());
	if(!output) {
		ErrorLog("CameraNode: output not found");
	}

	vec3 pos = self->params[0].Vec3();
	vec3 target = self->params[1].Vec3();

	output->LookAt(pos, target);
}

/* void SetupCameraNode(Node *self) */
/* { */
/* } */

ObjectHandle CreateCameraNode()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("pos", vec3(0, 0, 0)),
		NodeParameter("target", vec3(0, 0, 1)),
	};

	FixedArray<NodeInput> inputs = {
	};

	return AddNode(DATA_CAMERA, params, inputs);
}
