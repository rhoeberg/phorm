#pragma once


struct RenderObject {
	bool hasTexture;
	u32 textureHandle;
	int VAOHandle; // not a direct VAO id but an abstract handle
	GLuint VBO;
	GLuint EBO;
	i32 indexCount;
	i32 vertexCount;


	vec3 pos;
	vec3 scale;
	vec3 rot;
	vec4 color;

	GFXPrimitiveMode primitiveMode;
	f32 pointSize; 
	bool useIndices;
};
