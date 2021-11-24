#include "texture_graph.h"

void InitializeTextureGraph()
{
	_textureGraphState = (TextureGraphState*)malloc(sizeof(TextureGraphState));
	_textureGraphState->viewerQuad = createSpriteVAO();


	// TEST STUFF
	AddStack();
}

TextureStack* AddStack()
{
	TextureStack *stack = NULL;
	if(_textureGraphState->stackCount < MAX_TEXTURE_STACKS) {
		stack = &_textureGraphState->stacks[_textureGraphState->stackCount];
		_textureGraphState->stackCount++;
		glGenTextures(1, &stack->textureID);



		// TEST INITIALIZATION OF TEXTURE
		glBindTexture(GL_TEXTURE_2D, stack->textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		for(int i = 0; i < TEXTURE_PIXEL_AMOUNT; i++) {
			int val = i%255;
			stack->pixels[i] = Pixel(val, val, val, 255);
		}


		// INITIALIZE TEXTURE WITH IMAGE FILE
		// int width;
		// int height;
		// int nrChannels;
		// unsigned char *data = stbi_load("assets/pica.png", &width, &height, &nrChannels, STBI_rgb_alpha);
		// if(data) {
		// 	printf("width:%d\n", width);
		// 	printf("height:%d\n", height);
		// 	printf("nrChannels:%d\n", nrChannels);
		// 	memcpy(stack->pixels, data, sizeof(Pixel) * TEXTURE_PIXEL_AMOUNT);
		// 	free(data);
		// }

		glBindTexture(GL_TEXTURE_2D, stack->textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

	}

	return stack;
}

int GetPixelIndex(int x, int y)
{
	return ((y * TEXTURE_SIZE) + x);
}

void ShiftOperation(Pixel *pixels)
{
	for(int x = 0; x < TEXTURE_SIZE; x++) {
		for(int y = 0; y < TEXTURE_SIZE; y++) {
			int pixelIndex = GetPixelIndex(x, y);

			// if pixel at top of texture get from bottom
			int shiftPixelY = y;
			if(y == 0) {
				shiftPixelY = TEXTURE_SIZE - 1;
			}
			else {
				shiftPixelY = y - 1;
			}

			int shiftPixelIndex = GetPixelIndex(x, shiftPixelY);

			_textureGraphState->pixelBackBuffer[pixelIndex] = pixels[shiftPixelIndex];
		}
	}

	memcpy(pixels, _textureGraphState->pixelBackBuffer, TEXTURE_PIXEL_AMOUNT * sizeof(Pixel));
}


/*
  UL, UM, UR
  ML, MM, MR
  LL, LM, LR
 */
void BlurOperation(Pixel *pixels)
{
	for(int x = 0; x < TEXTURE_SIZE; x++) {
		for(int y = 0; y < TEXTURE_SIZE; y++) {
			int pixelIndex = GetPixelIndex(x, y);

			// for now ignore pixels around the edge
			if(x > 1 && x < TEXTURE_SIZE - 2 &&
			   y > 1 && y < TEXTURE_SIZE - 2) {
				// int neighbourUL = GetPixel(x - 1, y + 1);
				// int neighbourUM = GetPixel(x, y + 1);
				// int neighbourUR = GetPixel(x + 1, y + 1);

				// int neighbourML = GetPixel(x - 1, y);
				// // int neighbourUM = GetPixel(x, y);
				// int neighbourMR = GetPixel(x + 1, y);

				// int neighbourLL = GetPixel(x - 1, y - 1);
				// int neighbourLM = GetPixel(x, y - 1);
				// int neighbourLR = GetPixel(x + 1, y - 1);

				Pixel neighbours[8];
				neighbours[0] = pixels[GetPixelIndex(x - 1, y + 1)];
				neighbours[1] = pixels[GetPixelIndex(x, y + 1)];
				neighbours[2] = pixels[GetPixelIndex(x + 1, y + 1)];
				neighbours[3] = pixels[GetPixelIndex(x - 1, y)];
				neighbours[4] = pixels[GetPixelIndex(x + 1, y)];
				neighbours[5] = pixels[GetPixelIndex(x - 1, y - 1)];
				neighbours[6] = pixels[GetPixelIndex(x, y - 1)];
				neighbours[7] = pixels[GetPixelIndex(x + 1, y - 1)];

				int avgR = 0;
				int avgG = 0;
				int avgB = 0;
				for(int i = 0; i < ARRAY_SIZE(neighbours); i++) {
					avgR += neighbours[i].r;
					avgG += neighbours[i].g;
					avgB += neighbours[i].b;
				}
				avgR /= 8;
				avgG /= 8;
				avgB /= 8;
				pixels[pixelIndex].r = avgR;
				pixels[pixelIndex].g = avgG;
				pixels[pixelIndex].b = avgB;

				// Pixel neighbourUL = pixels[GetPixelIndex(x - 1, y + 1)];
				// Pixel neighbourUM = pixels[GetPixelIndex(x, y + 1)];
				// Pixel neighbourUR = pixels[GetPixelIndex(x + 1, y + 1)];
				// Pixel neighbourML = pixels[GetPixelIndex(x - 1, y)];
				// Pixel neighbourMR = pixels[GetPixelIndex(x + 1, y)];
				// Pixel neighbourLL = pixels[GetPixelIndex(x - 1, y - 1)];
				// Pixel neighbourLM = pixels[GetPixelIndex(x, y - 1)];
				// Pixel neighbourLR = pixels[GetPixelIndex(x + 1, y - 1)];


			}
		}
	}
}

void ProcessStack(TextureStack *stack)
{
	for(int i = 0; i < stack->nodeCount; i++) {
		TextureNode *node = &stack->nodes[i];
		node->operation(stack->pixels);
	}
}

void WriteStackToTexture(TextureStack *stack)
{
	glBindTexture(GL_TEXTURE_2D, stack->textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, stack->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
}

TextureStack* GetTextureStack(int handle)
{
	return &_textureGraphState->stacks[handle];
}

void UpdateTextureGraph()
{

	// TODO (rhoe) for now we just test the first stack if its there
	if(_textureGraphState->stackCount > 0) {

		// get the first stack
		TextureStack *stack = GetTextureStack(0);

		// ShiftOperation(stack->pixels);
		BlurOperation(stack->pixels);

		// generate texture
		WriteStackToTexture(stack);

		// render texture to quad here
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, stack->textureID);
		glUseProgram(GetTextureShader());

		glBindVertexArray(_textureGraphState->viewerQuad);
		glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
		GLuint modelLoc = glGetUniformLocation(GetTextureShader(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		// cleanup texture
		

	}
}

void CleanupTextureGraph()
{
	free(_textureGraphState);
}

