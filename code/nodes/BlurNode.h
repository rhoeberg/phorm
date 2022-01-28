#pragma once

void BlurOperation(Node *self)
{
	// INPUTS
	Texture *inputTexture = GetTextures()->Get(self->inputs[0]);
	if(!inputTexture)
		return;

	// SELF
	ObjectHandle handle = self->GetDataLast();
	Texture *output = GetTextures()->Get(handle);
	if(!output) {
		return;
	}

	output->Create(inputTexture->width, inputTexture->height);

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
	Pixel *backBuffer = (Pixel*)malloc(sizeof(Pixel) * inputTexture->width * inputTexture->height);
	// HORIZONTAL PASS
	for(int x = 0; x < output->width; x++) {
		for(int y = 0; y < output->height; y++) {
			int avgR = 0;
			int avgG = 0;
			int avgB = 0;
			int minX = (x - kernelWidth < 0) ? 0 : x - kernelWidth;
			int maxX = (x + kernelWidth > output->width - 1) ? output->width - 1 : x + kernelWidth;
			int divAmount = 0;
			int kernelIndex = 0;
			for(int i = minX; i < maxX; i++) {
				Pixel pixel = inputTexture->pixels[GetPixelIndex(i, y, output->width)];
				avgR += pixel.r * kernel[kernelIndex];
				avgG += pixel.g * kernel[kernelIndex];
				avgB += pixel.b * kernel[kernelIndex];
				divAmount += kernel[kernelIndex];
				kernelIndex++;
			}
			backBuffer[GetPixelIndex(x, y, output->width)].r = avgR / divAmount;
			backBuffer[GetPixelIndex(x, y, output->width)].g = avgG / divAmount;
			backBuffer[GetPixelIndex(x, y, output->width)].b = avgB / divAmount;

		}
	}

	// VERTICAL PASS
	for(int x = 0; x < output->width; x++) {
		for(int y = 0; y < output->height; y++) {
			int avgR = 0;
			int avgG = 0;
			int avgB = 0;
			int minY = (y - kernelWidth < 0) ? 0 : y - kernelWidth;
			int maxY = (y + kernelWidth > output->height - 1) ? output->height - 1 : y + kernelWidth;
			int divAmount = 0;
			int kernelIndex = 0;
			for(int i = minY; i < maxY; i++) {
				Pixel pixel = backBuffer[GetPixelIndex(x, i, output->width)];
				avgR += pixel.r * kernel[kernelIndex];
				avgG += pixel.g * kernel[kernelIndex];
				avgB += pixel.b * kernel[kernelIndex];
				divAmount += kernel[kernelIndex];
				kernelIndex++;
			}
			output->pixels[GetPixelIndex(x, y, output->width)].r = avgR / divAmount;
			output->pixels[GetPixelIndex(x, y, output->width)].g = avgG / divAmount;
			output->pixels[GetPixelIndex(x, y, output->width)].b = avgB / divAmount;

		}
	}
	free(backBuffer);
}

ObjectHandle CreateBlurTexture()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("amount", 20),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_TEXTURE),
	};

	return AddNode(DATA_TEXTURE, params, inputs);
}
