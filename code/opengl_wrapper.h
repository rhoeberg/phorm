#pragma once

#include "Bitmap.h"

/* struct OpenglContextState { */
/* 	VMArray<GLuint> VAOObjects; */
/* }; */

/* struct VAOHandle */
/* { */
/* 	u32 index; */
/* }; */

struct GFXTexture
{
	u32 id;
};

struct Buffer
{
	u32 id;
};

struct OpenglWrapperState
{
	// OpenglContextState mainWindowContext;
	// OpenglContextState viewerWindowContext;

	VMArray<u32> mainContextVAO;
	VMArray<u32> viewerContextVAO;
	VMArray<Buffer> buffers;
	VMArray<GFXTexture> textures;

	i32 defaultTexture;
	bool viewerInMain;
};


void InitializeOpenglWrapper();
int AddVAOMainWindow();
int AddVAOViewerWindow();
int AddVAO();

void ToggleViewer();
void SetViewerInMain(bool value);
bool ViewerInMain();
void SetContextMain();
void SetContextViewer();
void BindMainContextVAO(int handle);
void BindViewerContextVAO(int handle);
GLuint GetMainContextVAO(int handle);
GLuint GetViewerContextVAO(int handle);
GLuint GetCurrentContextVAO(int handle);

/* int AddStorageBuffer(); */
int AddStorageBuffer();

void BindBuffersToVAO(int VAOHandle, GLuint VBO, GLuint EBO);
/* void BindBufferToVAO(int handle, int type, GLuint bufferID); */
/* void SetVAOAttribPointers(int handle); */
int CreateSpriteVAO();
void CleanupOpenglWrapper();

GLuint CreateShader(const char *path, int type);
GLuint CreateShaderProgram(const char *vPath, const char *fPath);

///////////////
// GLOBAL
///////////////
void GFXClear(vec3 color);
void GFXSetViewport(i32 x, i32 y, i32 width, i32 height);

///////////////
// RESOURCES
///////////////

// TEXTURES
i32 GFXAddTexture();
void GFXTextureUploadBitmap(i32 handle, Bitmap *bitmap);
void GFXBindTexture(i32 handle);
void GFXBindDefaultTexture();

global OpenglWrapperState *_openglWrapperState;
