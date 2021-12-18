#pragma once

struct Mesh {
	VMArray<float> vertices;
	VMArray<GLuint> indices;

	void AddVertex(vec3 pos, vec2 uv, vec3 normal) {
		vertices.Insert(pos.x);
		vertices.Insert(pos.y);
		vertices.Insert(pos.z);
		vertices.Insert(uv.x);
		vertices.Insert(uv.y);
		vertices.Insert(normal.x);
		vertices.Insert(normal.y);
		vertices.Insert(normal.z);
	}

	void AddTriangle(i32 i1, i32 i2, i32 i3) {
		indices.Insert(i1);
		indices.Insert(i2);
		indices.Insert(i3);
	}
};
