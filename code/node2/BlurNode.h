#pragma once

Pixel* BlurOperation(TextureNode *self);

struct BlurNode {
	int amount;
};

int AddBlurNode()
{
	int nodeHandle = AddTextureNode();
	BlurNode blurNode = {};
	blurNode.amount = 10;
	int dataHandle = _nodeState->blurNodes.Insert(blurNode);

	TextureNode *textureNode = &_nodeState->textureNodes[nodeHandle];
	textureNode->dataHandle = dataHandle;
	textureNode->GetPixels = BlurOperation;

	NodeInput input = {};
	input.type = TEXTURE_NODE;
	input.handle = -1; // -1 means no input
	textureNode->inputs.Insert(input);

	return nodeHandle;
}

Pixel* BlurOperation(TextureNode *self)
{
	if(self->changed) {

		//TODO (rhoe) check if input is set
		TextureNode *inputNode = &_nodeState->textureNodes[self->inputs[0].handle];
		Pixel *inputPixels = inputNode->GetPixels(inputNode);

		// memcpy(self->pixels, inputNode->GetPixels(inputNode), sizeof(self->pixels));

		BlurNode *blurNode = &_nodeState->blurNodes[self->dataHandle];

		// int kernel[] = { 1, 2, 1 };
		// int kernel[] = { 1, 2, 4, 2, 1 };
		// int kernel[] = { 1, 2, 4, 8, 4, 2, 1 };
		int kernel[] = { 1, 2, 4, 8, 16, 8, 4, 2, 1 };
		int kernelWidth = (ARRAY_SIZE(kernel) - 1) / 2;

		Pixel *backBuffer = (Pixel*)malloc(sizeof(Pixel) * PIXEL_AMOUNT);

		for(int i = 0; i < blurNode->amount; i++) {

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
						Pixel pixel = inputPixels[GetPixelIndex(i, y)];
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
			// memcpy(self->pixels, backBuffer, sizeof(self->pixels));

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
					self->pixels[GetPixelIndex(x, y)].r = avgR / divAmount;
					self->pixels[GetPixelIndex(x, y)].g = avgG / divAmount;
					self->pixels[GetPixelIndex(x, y)].b = avgB / divAmount;

				}
			}
			memcpy(self->pixels, backBuffer, sizeof(self->pixels));
		}

		self->changed = false;
	}

	return self->pixels;

}

