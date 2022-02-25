#pragma once

#include "Bitmap.h"

/* struct OpenglContextState { */
/* 	VMArray<GLuint> VAOObjects; */
/* }; */

/* struct VAOHandle */
/* { */
/* 	u32 index; */
/* }; */

enum GFXBufferUsageType {
	GFX_STREAM_COPY,
	GFX_DYNAMIC_READ,
};

enum GFXBufferType {
	GFX_BUFFER_STORAGE,
	GFX_BUFFER_PBO,
	GFX_BUFFER_VBO,
};

enum GFXBarrierType {
	GFX_BARRIER_STORAGE,
	GFX_BARRIER_IMAGE,
};

struct GFXTexture
{
	u32 id;
};

struct GFXBuffer
{
	u32 id;
	GFXBufferType type;
};

struct OpenglWrapperState
{
	// OpenglContextState mainWindowContext;
	// OpenglContextState viewerWindowContext;

	VMArray<u32> mainContextVAO;
	VMArray<u32> viewerContextVAO;
	VMArray<GFXBuffer> buffers;
	VMArray<GFXTexture> textures;

	i32 defaultTexture;
	bool viewerInMain;
};

// TODO (rhoe) could change this one into a table lookup,
//             not sure it would make any difference though
GLenum GetBufferType(GFXBufferType type)
{
	switch(type) {
		case GFX_BUFFER_STORAGE: return GL_SHADER_STORAGE_BUFFER; break;
		case GFX_BUFFER_PBO: return GL_PIXEL_PACK_BUFFER; break;
		case GFX_BUFFER_VBO: return GL_ARRAY_BUFFER; break;
	}

	// TODO (rhoe) we have to return a default value to mute compiler warning
	return GL_ARRAY_BUFFER;
}

GLenum GetBufferUsageType(GFXBufferUsageType type)
{
	switch(type) {
		case GFX_STREAM_COPY: return GL_STREAM_COPY; break;
		case GFX_DYNAMIC_READ: return GL_DYNAMIC_READ; break;
	}

	// TODO (rhoe) we have to return a default value to mute compiler warning
	return GL_STREAM_COPY;
}

GLenum GetBarrierType(GFXBarrierType type)
{
	switch(type) {
		case GFX_BARRIER_STORAGE: return GL_SHADER_STORAGE_BARRIER_BIT; break;
		case GFX_BARRIER_IMAGE: return GL_SHADER_IMAGE_ACCESS_BARRIER_BIT; break;
	}

	// TODO (rhoe) we have to return a default value to mute compiler warning
	return GL_SHADER_IMAGE_ACCESS_BARRIER_BIT;
}

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
void GFXCompute(u32 x, u32 y, u32 z);
void GFXMemBarrier(GFXBarrierType type);
void GFXFinish();


///////////////
// RESOURCES
///////////////

// BUFFERS
i32 GFXBufferAdd(GFXBufferType type);
void GFXBufferSetData(u32 handle, u64 size, GFXBufferUsageType usage, void *data = NULL);
void GFXBufferBind(u32 handle);
void GFXBufferUnbind();
void GFXBufferBindBase(u32 handle, i32 base);
void* GFXBufferMap(u32 handle);
void GFXBufferUnmap(u32 handle);
void* GFXBufferMapRange(u32 handle, u64 size);

// TEXTURES
i32 GFXTextureAdd();
void GFXTextureUploadBitmap(u32 handle, Bitmap *bitmap);
void GFXTextureBind(u32 handle);
void GFXTextureBindBase(u32 handle, i32 base);
void GFXBindDefaultTexture();
void GFXTextureGetImage(u32 textureHandle, u32 pboHandle);

global OpenglWrapperState *_openglWrapperState;
