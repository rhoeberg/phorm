#pragma once

struct RenderState {
	GLuint cubeVAO;
	GLuint baseShader;
	GLuint textureShader;
};

void InitializeRender();
void CleanupRender();

void RenderCubeTexture_TMP(vec3 pos, GLuint texture);
void RenderCubeBase_TMP(vec3 pos);

global RenderState *_renderState;
