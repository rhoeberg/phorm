#pragma once

#define TEXTURE_CHANNEL_AMOUNT 4

struct Pixel {
	u8 r;
	u8 g;
	u8 b;
	u8 a;

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

struct Bitmap {
	bool initialized;
	u32 width;
	u32 height;
	Pixel *pixels;

	Bitmap();
	Bitmap(i32 _width, i32 _height, Pixel *_pixels);
	void Create(i32 _width, i32 _height);
	void Create(i32 _width, i32 _height, Pixel *_pixels);
	u64 GetMemSize();
};

i32 GetPixelIndex(i32 x, i32 y, i32 width);
