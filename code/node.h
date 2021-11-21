/*
  
  Objective oriented approach to the node system
  we have a base node class and from that we would make
  core node types like:
  - textureNode
  - renderObjectNode
  - materialNode
  - etc..
  

  using these core types as the base classes we can then create interesting
  nodes. like:
  - blur
  - slice
  - noise
  - etc..


  the core node types then provide a function (operation) which returns the type
  consumed by the next node in the chain.
  fx. a texture node provides a GetPixels function. 

  an example:
  a blur node is a texture node which have another texture node as input.
  when another node is then connected to the blur node and calls the GetPixels 
  function, the blur node then gets the pixels from its input by again calling
  GetPixels() and from that result it does the blur algorithm and returns that result
  futher down the chain of nodes.

  

  caching:
  we also have the possibility of caching the results if nothing in the chain above
  has changed.
  in our blur example this would mean that the first time its operation is executed
  it get the pixel from its input and does the algorithm, but the second time if
  nothing has changed in its input and nothing has changed in itself it simply returns
  the last calculated result.



 
  
 */

#pragma once

#define TEXTURE_SIZE 512
#define TEXTURE_CHANNEL_AMOUNT 4
#define PIXEL_AMOUNT (TEXTURE_SIZE * TEXTURE_SIZE)

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

class Node {
public:
	bool changed;

	Node() {
		changed = true;
	}
};

class TextureNode : public Node {
public:
	Pixel pixels[PIXEL_AMOUNT]; // maybe change this into a pointer?
	// void GetPixels(Pixel *out);
	virtual Pixel* GetPixels() = 0;
};

class LoadTextureNode : public TextureNode {
public:
	char path[128];

	// void GetPixels(Pixel *out) {
	// 	if(changed) {
	// 		LoadPixels();
	// 		changed = false;
	// 	}

	// 	memcpy(out, pixels, sizeof(pixels));
	// }

	Pixel* GetPixels() {
		if(changed) {
			LoadPixels();
			changed = false;
		}

		return pixels;
	}

	void LoadPixels() {
		int width;
		int height;
		int nrChannels;
		unsigned char *data = stbi_load("assets/pica.png", &width, &height, &nrChannels, STBI_rgb_alpha);
		if(data) {
			printf("width:%d\n", width);
			printf("height:%d\n", height);
			printf("nrChannels:%d\n", nrChannels);
			memcpy(pixels, data, sizeof(pixels));
			free(data);
		}
	}
};

int GetPixelIndex(int x, int y)
{
	return ((y * TEXTURE_SIZE) + x);
}

class BlurTextureNode : public TextureNode {
public:
	TextureNode *input;
	int amount;
	Pixel* GetPixels() {
		if(changed) {
			Pixel *inputPixels = input->GetPixels();
			memcpy(pixels, inputPixels, sizeof(pixels));
			Blur();
			changed = false;
		}
		return pixels;
	}

	void Blur() {

		// int kernel[] = { 1, 2, 1 };
		// int kernel[] = { 1, 2, 4, 2, 1 };
		// int kernel[] = { 1, 2, 4, 8, 4, 2, 1 };
		int kernel[] = { 1, 2, 4, 8, 16, 8, 4, 2, 1 };
		int kernelWidth = (ARRAY_SIZE(kernel) - 1) / 2;

		Pixel *backBuffer = (Pixel*)malloc(sizeof(Pixel) * PIXEL_AMOUNT);

		for(int i = 0; i < amount; i++) {

			// HORIZONTAL PASS
			for(int x = 0; x < TEXTURE_SIZE; x++) {
				for(int y = 0; y < TEXTURE_SIZE; y++) {

					int avgR = 0;
					int avgG = 0;
					int avgB = 0;
					int minX = (x - kernelWidth < 0) ? 0 : x - kernelWidth;
					int maxX = (x + kernelWidth > TEXTURE_SIZE - 1) ? TEXTURE_SIZE - 1 : x + kernelWidth;
					int divAmount = 0;
					int kernelIndex = 0;
					for(int i = minX; i < maxX; i++) {
						Pixel pixel = pixels[GetPixelIndex(i, y)];
						avgR += pixel.r * kernel[kernelIndex];
						avgG += pixel.g * kernel[kernelIndex];
						avgB += pixel.b * kernel[kernelIndex];
						divAmount += kernel[kernelIndex];
						kernelIndex++;
					}
					backBuffer[GetPixelIndex(x, y)].r = avgR / divAmount;
					backBuffer[GetPixelIndex(x, y)].g = avgG / divAmount;
					backBuffer[GetPixelIndex(x, y)].b = avgB / divAmount;

				}
			}
			memcpy(pixels, backBuffer, sizeof(pixels));

			// VERTICAL PASS
			for(int x = 0; x < TEXTURE_SIZE; x++) {
				for(int y = 0; y < TEXTURE_SIZE; y++) {

					int avgR = 0;
					int avgG = 0;
					int avgB = 0;
					int minY = (y - kernelWidth < 0) ? 0 : y - kernelWidth;
					int maxY = (y + kernelWidth > TEXTURE_SIZE - 1) ? TEXTURE_SIZE - 1 : y + kernelWidth;
					int divAmount = 0;
					int kernelIndex = 0;
					for(int i = minY; i < maxY; i++) {
						Pixel pixel = pixels[GetPixelIndex(x, i)];
						avgR += pixel.r * kernel[kernelIndex];
						avgG += pixel.g * kernel[kernelIndex];
						avgB += pixel.b * kernel[kernelIndex];
						divAmount += kernel[kernelIndex];
						kernelIndex++;
					}
					backBuffer[GetPixelIndex(x, y)].r = avgR / divAmount;
					backBuffer[GetPixelIndex(x, y)].g = avgG / divAmount;
					backBuffer[GetPixelIndex(x, y)].b = avgB / divAmount;

				}
			}
			memcpy(pixels, backBuffer, sizeof(pixels));
		}

		free(backBuffer);
	}

	BlurTextureNode() {
		amount = 1;
	}
};

class AddTextureNode : public TextureNode {
public:
	TextureNode *input1;
	TextureNode *input2;

	// 0 = input 1
	// 1 = input 2
	// 0.5 = equal mix
	float slider = 0.5f;

	Pixel* GetPixels() {
		if(changed) {
			// Pixel *inputPixels = input->GetPixels();
			// memcpy(pixels, inputPixels, sizeof(pixels));
			AddInputs();
			changed = false;
		}
		
		return pixels;
	}


	void AddInputs() {
		Pixel *pixel1 = input1->GetPixels();
		Pixel *pixel2 = input2->GetPixels();

		float inputMul1 = 1.0f - slider;
		float inputMul2 = slider;

		for(int x = 0; x < TEXTURE_SIZE; x++) {
			for(int y = 0; y < TEXTURE_SIZE; y++) {
				int index = GetPixelIndex(x, y);
				Pixel result = {};
				result.r = ((pixel1[index].r * inputMul1) + (pixel2[index].r * inputMul2));
				result.g = ((pixel1[index].g * inputMul1) + (pixel2[index].g * inputMul2));
				result.b = ((pixel1[index].b * inputMul1) + (pixel2[index].b * inputMul2));
				pixels[index] = result;
			}
		}

	}
};

