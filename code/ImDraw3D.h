#pragma once

struct ImDraw3DState
{
	PArray<GLfloat> vertices;
	vec4 drawColor;
	GLuint shader;
	GLuint vbo;
	int vaoHandle;
};

void ImDraw3DSetColor(vec4 color);
void ImDraw3DSetColor(vec3 color);

global ImDraw3DState *_im3D;
