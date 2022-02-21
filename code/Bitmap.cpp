#include "Bitmap.h"

Bitmap::Bitmap(i32 _width, i32 _height, Pixel *inputPixels)
{
	width = _width;
	height = _height;
	pixels = (Pixel*)malloc(sizeof(Pixel) * width * height);
	memcpy(pixels, inputPixels, sizeof(Pixel) * width * height);
	initialized = true;
}

/*
  Allocated a bitmap of size width & height
  if bitmap of same size is already allocated then dont do anything
*/
void Bitmap::Create(i32 _width, i32 _height)
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

void Bitmap::Create(i32 _width, i32 _height, Pixel *inputPixels)
{
	if(!initialized) {
		width = _width;
		height = _height;
		memcpy(pixels, inputPixels, sizeof(Pixel) * width * height);
		initialized = true;
	} 
	else {
		if(_width != width || _height != height) {
			free(pixels);
			width = _width;
			height = _height;
			memcpy(pixels, inputPixels, sizeof(Pixel) * width * height);
		}
	}
}

int GetPixelIndex(i32 x, i32 y, i32 width)
{
	return ((y * width) + x);
}
