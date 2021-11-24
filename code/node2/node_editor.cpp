#include "node_editor.h"

#define VIEWER_SIZE 300

void InitializeNodeEditor()
{
	_nodeEditorState = (NodeEditorState*)malloc(sizeof(NodeEditorState));
	_nodeEditorState->isDragging = false;
	_nodeEditorState->draggedNodeHandle = -1;
	_nodeEditorState->hoverState.hoveringElement = false;




	// create texture for node
	glGenTextures(1, &_nodeEditorState->textureID);
	glBindTexture(GL_TEXTURE_2D, _nodeEditorState->textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create quad to render texture to
	_nodeEditorState->viewerQuad = createSpriteVAO();


	// create fbo + fbo texture
	glGenTextures(1, &_nodeEditorState->fboTexture);
	glBindTexture(GL_TEXTURE_2D, _nodeEditorState->fboTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, VIEWER_SIZE, VIEWER_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &_nodeEditorState->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _nodeEditorState->fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _nodeEditorState->fboTexture, 0);  
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	// create viewer shader
	_nodeEditorState->viewerShader = createShaderProgram("assets/viewer.vert", "assets/viewer.frag");
	glUseProgram(_nodeEditorState->viewerShader);
    GLuint projectionLoc = glGetUniformLocation(_nodeEditorState->viewerShader, "projection");
    glm::mat4 projection;
    projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    GLuint modelLoc = glGetUniformLocation(_nodeEditorState->viewerShader, "model");
	glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
    model = glm::translate(model, glm::vec3(SCREEN_WIDTH - (VIEWER_SIZE + 10), 10.0f, 0.0f));
    model = glm::scale(model, glm::vec3(VIEWER_SIZE, VIEWER_SIZE,1));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void DrawNode(int handle)
{
	Node *node = &_nodeState->nodes[handle];
	ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
	ImDrawRect(node->rect);

	for(int i = 0; i < node->inputs.Count(); i++) {
		NodeInput input = node->inputs[i];
		Node *inputNode = &_nodeState->nodes[input.handle];
		ImDrawLine(inputNode->rect.pos, node->rect.pos);
	}
}

void UpdateHoverState()
{
	_nodeEditorState->hoverState.hoveringElement = false;
	for(int i = 0; i < _nodeState->nodes.Count(); i++) {
		if(PointInsideRect(mouse, _nodeState->nodes[i].rect)) {
			_nodeEditorState->hoverState.nodeHandle = i;
			_nodeEditorState->hoverState.hoveringElement = true;
		}
	}
}

void ShowNode(Node *node)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _nodeEditorState->fbo);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
    glViewport(0, 0, VIEWER_SIZE, VIEWER_SIZE);

	switch(node->type) {
		case TEXTURE_NODE: {
			int dataHandle = RunNodeOperation(node);
			Texture *texture = &_nodeState->textures[dataHandle];

			glBindTexture(GL_TEXTURE_2D, _nodeEditorState->textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
			glBindTexture(GL_TEXTURE_2D, 0);

			// render texture to quad here
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _nodeEditorState->textureID);
			glUseProgram(GetTextureShader());

			// glUseProgram(_nodeEditorState->viewerShader);
			glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
			// TODO (rhoe) temporary hacky setup to get the image rendered nicely in the viewer
			model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));
			GLuint modelLoc = glGetUniformLocation(GetTextureShader(), "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			// model = glm::scale(model, glm::vec3(VIEWER_SIZE, VIEWER_SIZE,1));
			glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)VIEWER_SIZE / (float)VIEWER_SIZE, 0.1f, 1000.0f);
			GLuint projectionLoc = glGetUniformLocation(GetTextureShader(), "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
			glBindVertexArray(_nodeEditorState->viewerQuad);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			break;
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void UpdateNodeDragging()
{
	if(_nodeEditorState->hoverState.hoveringElement) {
		if(mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] && !_nodeEditorState->isDragging) {
			_nodeEditorState->draggedNodeHandle = _nodeEditorState->hoverState.nodeHandle;
			_nodeEditorState->isDragging = true;
		}
	}

	if(_nodeEditorState->isDragging) {

		if(!mouse_buttons[GLFW_MOUSE_BUTTON_LEFT]) {
			// handle stopped dragging
			_nodeEditorState->isDragging = false;
		}
		else {
			// handle continued dragging
			_nodeState->nodes[_nodeEditorState->draggedNodeHandle].rect.pos = mouse;
		}
	}
}

void UpdateNodeEditor()
{
	//////////////////
	// CHECK HOVERSTATE
	UpdateHoverState();

	UpdateNodeDragging();

	//////////////////
	// DRAW NODES
    // glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	for(int i = 0; i < _nodeState->nodes.Count(); i++) {
		DrawNode(i);
	}


	//////////////////
	// VIEWER
	if(_nodeEditorState->draggedNodeHandle != -1) {
		ShowNode(&_nodeState->nodes[_nodeEditorState->draggedNodeHandle]);
	}

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glBindTexture(GL_TEXTURE_2D, _nodeEditorState->fboTexture);
    glUseProgram(_nodeEditorState->viewerShader);
	glBindVertexArray(_nodeEditorState->viewerQuad);
	// glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	// GLuint modelLoc = glGetUniformLocation(GetTextureShader(), "model");
	// glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
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

void CleanupNodeEditor()
{
	free(_nodeEditorState);
}
