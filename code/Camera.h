#pragma once

enum CamDir {
	CAM_FORWARD,
	CAM_BACKWARD,
	CAM_LEFT,
	CAM_RIGHT,
};

struct Camera {
	vec3 pos;
	vec3 front;
	float orbitDist;
	float speed;

	Camera()
	{
		pos = vec3(0, 0, -3);
		front = vec3(0, 0, 1);

		orbitDist = 5.0f;
		speed = 10.0f;
	}

	mat4 GetViewMatrix()
	{
		mat4 view = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
		view = glm::lookAt(pos, pos + front, WORLD_UP);
		return view;
	}

	void OrbitDrag(vec2 dragAmount)
	{
		float x = orbitDist * sin(-dragAmount.x) * cos(dragAmount.y);
		float y = orbitDist * sin(dragAmount.y);
		float z = orbitDist * cos(dragAmount.y) * cos(-dragAmount.x);
		vec3 newPos = pos + vec3(x, y, z);

		vec3 orbitCenter = pos + (front * orbitDist);

		front = glm::normalize(newPos - orbitCenter);

		pos = newPos;
	}

	void Move(CamDir dir)
	{
		vec3 vel = vec3(0, 0, 0);
		float deltaSpeed = speed * deltaTime;

		vec3 right = glm::normalize(glm::cross(front, WORLD_UP));

		if(dir == CAM_FORWARD) {
			vel += front * deltaSpeed;
		}
		if(dir == CAM_BACKWARD) {
			vel -= front * deltaSpeed;
		}
		if(dir == CAM_LEFT) {
			vel -= right * deltaSpeed;
		}
		if(dir == CAM_RIGHT) {
			vel += right * deltaSpeed;
		}

		pos += vel;
	}
	
};
