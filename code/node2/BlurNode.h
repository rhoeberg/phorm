#pragma once

void BlurOperation(Node *self)
{
	// INPUTS
	Texture *inputTexture = GetTextureInput(self->inputs[0]);
	if(!inputTexture)
		return;

	// SELF
	Texture *output = GetTexture(self->GetDataLast());

	// OPERATION
	// int kernel[] = { 1, 2, 1 };
	// int kernel[] = { 1, 2, 4, 2, 1 };
	// int kernel[] = { 1, 2, 4, 8, 4, 2, 1 };
	/* int kernel[] = { 1, 2, 4, 8, 16, 8, 4, 2, 1 }; */
	/* int kernel[] = { 1, 2, 4, 8, 16, 32, 16, 8, 4, 2, 1 }; */
	int kernel[] = { 16, 2, 3, 4, 5, 6, 5, 4, 3, 2, 16 };
	/* int kernel[] = {16, 2, 1, 2, 16}; */
	int kernelWidth = (ARRAY_SIZE(kernel) - 1) / 2;

	// TODO (rhoe) easy optimization would be to have a shared
	//             backbuffer that gets malloced once in the beginning
	Pixel *backBuffer = (Pixel*)malloc(sizeof(Pixel) * PIXEL_AMOUNT);
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
				Pixel pixel = inputTexture->pixels[GetPixelIndex(i, y)];
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
				Pixel pixel = backBuffer[GetPixelIndex(x, i)];
				avgR += pixel.r * kernel[kernelIndex];
				avgG += pixel.g * kernel[kernelIndex];
				avgB += pixel.b * kernel[kernelIndex];
				divAmount += kernel[kernelIndex];
				kernelIndex++;
			}
			output->pixels[GetPixelIndex(x, y)].r = avgR / divAmount;
			output->pixels[GetPixelIndex(x, y)].g = avgG / divAmount;
			output->pixels[GetPixelIndex(x, y)].b = avgB / divAmount;

		}
	}
}

int AddBlurNode()
{
	VMArray<NodeParameter> params = {
		NodeParameter("amount", PARAM_INT, 20),
	};

	VMArray<NodeInput> inputs = {
		NodeInput(TEXTURE_NODE),
	};

	return AddNode("BLUR", TEXTURE_NODE, BlurOperation, params, inputs);
}
