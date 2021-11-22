#include "node2.h"

int GetPixelIndex(int x, int y)
{
	return ((y * TEXTURE_SIZE) + x);
}

TextureNode* GetTextureNode(int handle)
{
	if(handle > ARRAY_SIZE(_nodeState->textureNodes) - 1 ||
	   handle < 0) {
		// invalid handle
		return NULL;
	}

	return &_nodeState->textureNodes[handle];
}

/////////////
// NODE OPS
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

Pixel* LoadTextureOperation(TextureNode *self)
{
	if(self->changed) {
		int width;
		int height;
		int nrChannels;
		unsigned char *data = stbi_load("assets/pica.png", &width, &height, &nrChannels, STBI_rgb_alpha);
		if(data) {
			printf("width:%d\n", width);
			printf("height:%d\n", height);
			printf("nrChannels:%d\n", nrChannels);
			memcpy(self->pixels, data, sizeof(self->pixels));
			free(data);
		}

		self->changed = false;
	}

	return self->pixels;
}


int AddTextureNode()
{
	if(_nodeState->textureNodeCount < ARRAY_SIZE(_nodeState->textureNodes)) {
		int handle = _nodeState->textureNodeCount;
		_nodeState->textureNodeCount++;

		// set default texture node state
		TextureNode *node = GetTextureNode(handle);
		node->pos = vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		node->changed = true;

		return handle;
	}
	return -1;
}

void AddNodeInput(int handle, NodeType type)
{
	TextureNode *node = GetTextureNode(handle);
	if(node->inputCount < 0 || node->inputCount > ARRAY_SIZE(node->inputs) - 1) {
		// cannot add node input
		return;
	}

	node->inputs[node->inputCount].type = type;
	node->inputs[node->inputCount].handle = -1; // for now we use -1 for unused input
	node->inputCount++;
}

int AddBlurNode()
{
	int nodeHandle = AddTextureNode();
	if(nodeHandle > -1) {
		if(_nodeState->blurNodeCount < ARRAY_SIZE(_nodeState->blurNodes)) {
			int dataHandle = _nodeState->blurNodeCount;
			_nodeState->blurNodeCount++;

			TextureNode *textureNode = GetTextureNode(nodeHandle);
			textureNode->dataHandle = dataHandle;
			textureNode->GetPixels = BlurOperation;

			// blur node
			BlurNode *blurNode = &_nodeState->blurNodes[dataHandle];
			blurNode->amount = 10;

			AddNodeInput(nodeHandle, TEXTURE_NODE);

			return nodeHandle;
		}
	}

	return -1;
}

int AddLoadTextureNode()
{
	int nodeHandle = AddTextureNode();
	if(nodeHandle > -1) {
		if(_nodeState->loadTextureNodeCount < ARRAY_SIZE(_nodeState->loadTextureNodes)) {
			int dataHandle = _nodeState->loadTextureNodeCount;
			_nodeState->loadTextureNodeCount++;

			TextureNode *textureNode = GetTextureNode(nodeHandle);
			textureNode->dataHandle = dataHandle;
			textureNode->GetPixels = LoadTextureOperation;

			return nodeHandle;
		}
	}

	return -1;
}



/////////////////
// INITIALIZATION

void InitializeNodes()
{
	_nodeState = (NodeState*)malloc(sizeof(NodeState));
	_nodeState->textureNodeCount = 0;
	_nodeState->blurNodeCount = 0;
	_nodeState->loadTextureNodeCount = 0;





	// TESTING STUFF
	int loadHandle = AddLoadTextureNode();
	_nodeState->blurNodeHandle = AddBlurNode();
	TextureNode *blurNode = GetTextureNode(_nodeState->blurNodeHandle);
	blurNode->inputs[0].type = TEXTURE_NODE;
	blurNode->inputs[0].handle = loadHandle;
	// BlurNode *blu = GetTextureNode(loadHandle);


	glGenTextures(1, &_nodeState->textureID);
	glBindTexture(GL_TEXTURE_2D, _nodeState->textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	_nodeState->viewerQuad = createSpriteVAO();
}

void UpdateNodes()
{
	//////////////////
	// DRAW NODES
	for(int i = 0; i < _nodeState->textureNodeCount; i++) {
		TextureNode *node = GetTextureNode(i);
		Rect rect = {};
		rect.pos = node->pos;
		// rect.pos = vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		rect.width = 50;
		rect.height = 30;
		ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
		ImDrawRect(rect);
	}


	///////////////////
	// TESTING STUFF
	TextureNode *node = &_nodeState->textureNodes[_nodeState->blurNodeHandle];
	Pixel *pixels = node->GetPixels(node);

	glBindTexture(GL_TEXTURE_2D, _nodeState->textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	// render texture to quad here
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _nodeState->textureID);
	glUseProgram(GetTextureShader());

	glBindVertexArray(_nodeState->viewerQuad);
	glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	GLuint modelLoc = glGetUniformLocation(GetTextureShader(), "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void NodeGUI()
{
	ImGui::Begin("Inspector");
	if(ImGui::Button("add blur node")) {
		AddBlurNode();
	}
	ImGui::End();
}

void CleanupNodes()
{
	free(_nodeState);
}


