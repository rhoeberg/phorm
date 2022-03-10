#pragma once

enum GFXBufferType {
	BUFFER_VERTEX,
	BUFFER_INDICES,
	BUFFER_STORAGE,
};
		

/* struct GFXTexture */
/* { */
/* 	void *apiData; */
/* 	u32 width; */
/* 	u32 height; */
/* }; */

struct GFXBuffer
{
	u32 apiHandle;
	GFXBufferType type;
	u64 size;
};

struct GFXState
{
	PArray<GFXTexture> textures;
	PArray<GFXBuffer> buffers;
};

void GFXLayerInit();
void CreateTexture();

// BUFFERS
void GFXCreateBuffer(GFXBufferType type, u64 size, u32 stride);
void GFXBindBuffer(u32 handle, u32 binding);


global GFXState *_gfx;
