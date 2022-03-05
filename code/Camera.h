#pragma once

enum CamDir {
	CAM_FORWARD,
	CAM_BACKWARD,
	CAM_LEFT,
	CAM_RIGHT,
};

struct Camera {
	// base attributes
	vec3 pos;
	vec3 front;

	// orbit
	vec3 orbitCenter;
	float orbitDist;
	float orbitDragSpeed;

	// movement
	float moveSpeed;
	float lookDirSpeed;

	Camera()
	{
		Reset();
	}

	void Reset()
	{
		pos = vec3(0, 0, -2);
		front = vec3(0, 0, 1);
		orbitDist = 2.0f;
		orbitDragSpeed = 0.01f;
		orbitCenter = vec3(0, 0, 0);

		moveSpeed = 3.0f;
		lookDirSpeed = 0.005f;
	}

	mat4 GetViewMatrix()
	{
		mat4 view = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
		view = glm::lookAt(pos, pos + front, WORLD_UP);
		return view;
	}

	void OrbitScrollOut(float amount)
	{
		if((orbitDist - amount) > 0.0f) {
			orbitDist -= amount;
			pos = pos + (front * orbitDist);
		}
	}

	// Returns pitch in degrees
	double Pitch()
	{
		// GET PITCH
		vec3 frontX0Z = glm::normalize(front * vec3(1, 0, 1));
		double angle = glm::angle(front, frontX0Z);
		return glm::degrees(angle);
	}

	vec3 CamRight()
	{
		vec3 result = glm::normalize(glm::cross(front, WORLD_UP));
		return result;
	}

	vec3 CamUp()
	{
		vec3 result = glm::normalize(glm::cross(CamRight(), front));
		return result;
	}

	void OrbitDrag(vec2 dragOffset)
	{
		orbitCenter = pos + (front * orbitDist);

		vec3 camFocus = glm::normalize(pos - orbitCenter) * orbitDist;
		float yawAmount = orbitDragSpeed * dragOffset.x;
		float pitchAmount = orbitDragSpeed * dragOffset.y;

		// multiply yaw by pitch value to compensate for smaller yaw
		// distance when looking "down" or "up" at center
		float yawMult = ((90 - Pitch()) / 90);
		yawAmount = yawAmount * yawMult;

		// do the actualy rotation of the orbit focus point
		mat3 yawMat = glm::rotate(yawAmount, CamUp());
		mat3 pitchMat = glm::rotate(pitchAmount, CamRight());
		camFocus = camFocus * pitchMat * yawMat;

		// set the new pos and front vectors
		pos = camFocus + orbitCenter;
		front = glm::normalize(orbitCenter - pos);
	}

	void MouseLook(vec2 offset)
	{
		float yawAmount = lookDirSpeed * offset.x;
		float pitchAmount = lookDirSpeed * offset.y;
		mat3 yawMat = glm::rotate(yawAmount, CamUp());
		mat3 pitchMat = glm::rotate(pitchAmount, CamRight());
		front = glm::normalize(front * pitchMat * yawMat);
	}

	void Move(CamDir dir)
	{
		vec3 right = glm::normalize(glm::cross(front, WORLD_UP));

		vec3 vel = vec3(0, 0, 0);
		if(dir == CAM_FORWARD) {
			vel += front;
		}
		if(dir == CAM_BACKWARD) {
			vel -= front;
		}
		if(dir == CAM_LEFT) {
			vel -= right;
		}
		if(dir == CAM_RIGHT) {
			vel += right;
		}

		float deltaSpeed = moveSpeed * deltaTime;
		pos += glm::normalize(vel) * deltaSpeed;
	}

	void LookAt(vec3 _pos, vec3 target)
	{
		pos = _pos;
		front = normalize(target - _pos);
	}
};
