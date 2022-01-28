#pragma once

/* #define TEXTURE_SIZE 512 */
#define TEXTURE_CHANNEL_AMOUNT 4
/* #define PIXEL_AMOUNT (TEXTURE_SIZE * TEXTURE_SIZE) */

struct Pixel {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	Pixel() {
		r = g = b = a = 0;
	}

	Pixel(unsigned int _r, unsigned int _g, unsigned int _b, unsigned int _a) {
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
};

struct Texture {
	bool initialized;
	u32 width;
	u32 height;
	Pixel *pixels;

	void Create(u32 _width, u32 _height);
};

int GetPixelIndex(i32 x, i32 y, i32 height);
