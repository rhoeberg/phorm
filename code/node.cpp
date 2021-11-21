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
}

void UpdateNodes()
{

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


}

void NodeGUI()
{
    bool show = true;

    ImGui::Begin("node gui", &show);
	if(ImGui::InputInt("blur amount", &blurTex->amount)) {
		blurTex->changed = true;
	}
    ImGui::End();
}

void CleanupNodes()
{
	free(loadTex);
	free(blurTex);
	free(addTex);
}
