#pragma once
struct RenderObject {
	bool hasTexture;
	GLuint textureID;
	int VAOHandle; // not a direct VAO id but an abstract handle
	GLuint VBO;
	GLuint EBO;
	int indicesCount;

	vec3 pos;
	vec3 scale;
	/* quat rot; */
	vec3 rot;
	vec4 color;
};
