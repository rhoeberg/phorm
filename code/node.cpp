#include "node.h"

global LoadTextureNode *loadTex;
global BlurTextureNode *blurTex;
global AddTextureNode *addTex;
global GLuint textureID;
global GLuint viewerQuad;

void InitializeNodes()
{
	// loadTex = (LoadTextureNode*)malloc(sizeof(LoadTextureNode));
	// loadTex->changed = true;
	loadTex = new LoadTextureNode();
	loadTex->changed = true;

	// blurTex = (BlurTextureNode*)malloc(sizeof(BlurTextureNode));
	blurTex = new BlurTextureNode();
	blurTex->changed = true;
	blurTex->amount = 10;
	blurTex->input = loadTex;

	addTex = new AddTextureNode();
	addTex->input1 = loadTex;
	addTex->input2 = blurTex;
	// addTex->chagned = true;


	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	
	viewerQuad = createSpriteVAO();

	// free(loadTex);


	// _nodeState = (NodeState*)malloc(sizeof(NodeState));
}

void UpdateNodes()
{

	/////////////////////////
	// TEST IMPLEMENTATION
	/////////////////////////

	// Pixel *pixels = blurTex->GetPixels();
	float modSlider = abs(sin(glfwGetTime() * 2));

	addTex->slider = modSlider;
	addTex->changed = true;

	Pixel *pixels = addTex->GetPixels();

	// Pixel *pixels = NULL;

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	// render texture to quad here
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUseProgram(GetTextureShader());

	glBindVertexArray(viewerQuad);
	glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	GLuint modelLoc = glGetUniformLocation(GetTextureShader(), "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


	/////////////////////////
	// UPDATE NODES
	/////////////////////////
	for(int i = 0; i < _nodeState.textureNodes.size(); i++) {
		_nodeState.textureNodes[i]->Update();
	}


	/////////////////////////
	// RENDER NODES
	/////////////////////////
	for(int i = 0; i < _nodeState.textureNodes.size(); i++) {
		_nodeState.textureNodes[i]->RenderNode();
	}


}

void NodeGUI()
{
    bool show = true;

    ImGui::Begin("node gui", &show);

	if(ImGui::Button("add blur node")) {
		BlurTextureNode *node = new BlurTextureNode();
		node->SetName("Blur");
		_nodeState.textureNodes.push_back(node);
	}

    ImGui::End();

	if(_nodeState.selectedNode != NULL) {
		_nodeState.selectedNode->ShowGUI();
	}
}

void CleanupNodes()
{
	delete loadTex;
	delete blurTex;
	delete addTex;

	for(int i = 0; i < _nodeState.textureNodes.size(); i++) {
		delete _nodeState.textureNodes[i];
	}
	// free(_nodeState);
}

////////////////
// NODE
////////////////
Node::Node()
{
	changed = true;
	dragging = false;
	rect.pos = vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	rect.width = 50;
	rect.height = 30;
}

void Node::RenderNode()
{
	ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
	ImDrawRect(rect);

	// DRAW NAME
	ImDrawText(rect.pos + vec2(5, rect.height - 5), name);
}


void Node::Update()
{
	if(mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] && !_nodeState.isDragging) {
		if(PointInsideRect(mouse, rect)) {
			_nodeState.isDragging = true;
			dragging = true;
			_nodeState.selectedNode = this;
		}
	}
	else if(!mouse_buttons[GLFW_MOUSE_BUTTON_LEFT]) {
		_nodeState.isDragging = false;
		dragging = false;
	}

	if(dragging) {
		rect.pos = mouse;
	}
}

void Node::SetName(const char *newName)
{
	sprintf(name, "%s", newName);
}

////////////////
// BLUR TEXTURE NODE
////////////////
BlurTextureNode::BlurTextureNode()
{
	amount = 1;
}

Pixel* BlurTextureNode::GetPixels()
{
	if(changed) {
		Pixel *inputPixels = input->GetPixels();
		memcpy(pixels, inputPixels, sizeof(pixels));
		Blur();
		changed = false;
	}
	return pixels;
}

void BlurTextureNode::Blur()
{
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

void BlurTextureNode::ShowGUI()
{
	ImGui::Begin("BlurNode");
	ImGui::InputInt("amount", &amount);
	ImGui::End();
}


////////////////
// ADD TEXTURE NODE
////////////////
Pixel* AddTextureNode::GetPixels()
{
	if(changed) {
		// Pixel *inputPixels = input->GetPixels();
		// memcpy(pixels, inputPixels, sizeof(pixels));
		AddInputs();
		changed = false;
	}
		
	return pixels;
}

void AddTextureNode::AddInputs()
{
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

void AddTextureNode::ShowGUI()
{
	ImGui::Begin("Inspector");
	ImGui::Text("hello there");
	ImGui::End();
}


////////////////
// LOAD TEXTURE NODE
////////////////
Pixel* LoadTextureNode::GetPixels()
{
	if(changed) {
		LoadPixels();
		changed = false;
	}

	return pixels;
}

void LoadTextureNode::LoadPixels()
{
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

void LoadTextureNode::ShowGUI()
{
	ImGui::Begin("Inspector");
	ImGui::Text("hello there");
	ImGui::End();
}
