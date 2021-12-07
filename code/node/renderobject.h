#pragma once

struct RenderObject {
	bool hasTexture;
	GLuint textureID;
	int VAOHandle; // not a direct VAO id but an abstract handle
	int VAO; // not a direct VAO id but an abstract handle
	GLuint VBO;
	GLuint EBO;
	int indicesCount;
};
