#include "texture.h"

/*
  Allocated a texture of size width & height
  if texture of same size is already allocated then dont do anything
*/
void Texture::Create(u32 _width, u32 _height)
{
	if(!initialized) {
		width = _width;
		height = _height;
		pixels = (Pixel*)malloc(sizeof(Pixel) * width * height);
		initialized = true;
	} 
	else {
		if(_width != width || _height != height) {
			free(pixels);
			width = _width;
			height = _height;
			pixels = (Pixel*)malloc(sizeof(Pixel) * width * height);
		}
	}
}

int GetPixelIndex(i32 x, i32 y, i32 width)
{
	return ((y * width) + x);
}
