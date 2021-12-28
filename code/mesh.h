#pragma once

struct Vertex {
	float vx;
	float vy;
	float vz;
	float ux;
	float uy;
	float nx;
	float ny;
	float nz;

	Vertex() {}

	Vertex(float _vx, float _vy, float _vz, float _ux, float _uy, float _nx, float _ny, float _nz)
	{
		vx = _vx;
		vy = _vy;
		vz = _vz;
		ux = _ux;
		uy = _uy;
		nx = _nx;
		ny = _ny;
		nz = _nz;
	}

	vec3 Pos()
	{
		return vec3(vx, vy, vz);
	}
};

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

	void AddVertex(float vx, float vy, float vz, float ux, float uy, float nx, float ny, float nz) {
		vertices.Insert(vx);
		vertices.Insert(vy);
		vertices.Insert(vz);
		vertices.Insert(ux);
		vertices.Insert(uy);
		vertices.Insert(nx);
		vertices.Insert(ny);
		vertices.Insert(nz);
	}

	void AddVertex(Vertex vertex) {
		vertices.Insert(vertex.vx);
		vertices.Insert(vertex.vy);
		vertices.Insert(vertex.vz);
		vertices.Insert(vertex.ux);
		vertices.Insert(vertex.uy);
		vertices.Insert(vertex.nx);
		vertices.Insert(vertex.ny);
		vertices.Insert(vertex.nz);
	}

	void AddTriangle(i32 i1, i32 i2, i32 i3) {
		indices.Insert(i1);
		indices.Insert(i2);
		indices.Insert(i3);
	}

	Vertex GetVertex(i32 i) {
		i32 step = i * 8;

		Vertex result = {};
		result.vx = vertices[step];
		result.vy = vertices[step + 1];
		result.vz = vertices[step + 2];
		result.ux = vertices[step + 3];
		result.uy = vertices[step + 4];
		result.nx = vertices[step + 5];
		result.ny = vertices[step + 6];
		result.nz = vertices[step + 7];

		return result;
	}

	void SetNormalAt(i32 i, vec3 normal) {
		i32 step = i * 8;
		vertices[step+5] = normal.x;
		vertices[step+6] = normal.y;
		vertices[step+7] = normal.z;
	}
};
