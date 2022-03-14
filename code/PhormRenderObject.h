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

struct RenderObjectInstance {
	ObjectHandle renderObjectHandle;
	vec3 pos;
	vec3 scale;
	vec3 rot;
	vec4 color;

	RenderObjectInstance() {}

	RenderObjectInstance(ObjectHandle _renderObjectHandle)
	{
		renderObjectHandle = _renderObjectHandle;
		pos = vec3(0, 0, 0);
		scale = vec3(1, 1, 1);
		rot = vec3(0, 0, 0);
		color = vec4(1, 1, 1, 1);
	}

	RenderObjectInstance(ObjectHandle _renderObjectHandle, vec3 _pos, vec3 _scale, vec3 _rot, vec4 _color)
	{
		renderObjectHandle = _renderObjectHandle;
		pos = _pos;
		scale = _scale;
		rot = _rot;
		color = _color;
	}

	void Render(Shader *shader);
};
