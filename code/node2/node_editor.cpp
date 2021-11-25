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

#define PARAM_WIDTH 40
#define PARAM_HEIGHT 20

Rect GetNodeRect(int handle)
{
	Node *node = GetNode(handle);

	Rect rect = {};
	rect.pos = node->pos;
	rect.width = 100;
	rect.width += node->params.Count() * PARAM_WIDTH;
	rect.width += node->inputs.Count() * PARAM_WIDTH;
	rect.height = 30;

	return rect;
}

Rect GetNodeOutputRect(int handle)
{
	Rect nodeRect = GetNodeRect(handle);

	// DRAW OUTPUT
	Rect result = {};
	float outputOffsetX = (nodeRect.width / 2.0f) - PARAM_WIDTH / 2.0f;
	result.pos = nodeRect.pos + vec2(outputOffsetX, nodeRect.height);
	result.width = PARAM_WIDTH;
	result.height = PARAM_HEIGHT;

	return result;
}

void DrawNode(int handle)
{
	Node *node = GetNode(handle);

	Rect rect = GetNodeRect(handle);
	ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
	ImDrawRect(rect);

	vec2 namePos = node->pos + vec2(10.0f, rect.height - 10.0f);
	ImDrawText(namePos, node->name);

	Rect outputRect = GetNodeOutputRect(handle);
	ImDrawSetColor(vec3(0.0f, 0.0f, 1.0f));
	ImDrawRect(outputRect);

	// DRAW INPUTS
	for(int i = 0; i < node->inputs.Count(); i++) {
		Rect inputRect = {};
		float offsetX = i * (PARAM_WIDTH + 10);
		float offsetY = -PARAM_HEIGHT;
		inputRect.pos = rect.pos + vec2(offsetX, offsetY);
		inputRect.width = PARAM_WIDTH;
		inputRect.height = PARAM_HEIGHT;
		ImDrawSetColor(vec3(1.0f, 0.0f, 0.0f));
		ImDrawRect(inputRect);

		Rect outputRect = GetNodeOutputRect(node->inputs[i].handle);
		vec2 outputPos = GetRectCenter(outputRect);
		vec2 inputPos = GetRectCenter(inputRect);
		ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
		ImDrawLine(outputPos, inputPos);
	}

	// DRAW PARAMETERS
	int visibleParamsCount = 0;
	for(int i = 0; i < node->params.Count(); i++) {
		if(node->params[i].exposed) {
			Rect paramRect = {};
			float offsetX = (visibleParamsCount + node->inputs.Count()) * (PARAM_WIDTH + 10);
			float offsetY = -PARAM_HEIGHT;
			paramRect.pos = rect.pos + vec2(offsetX, offsetY);
			paramRect.width = PARAM_WIDTH;
			paramRect.height = PARAM_HEIGHT;
			ImDrawSetColor(vec3(0.0f, 1.0f, 0.0f));
			ImDrawRect(paramRect);
			visibleParamsCount++;
		}
	}
}

void UpdateHoverState()
{
	_nodeEditorState->hoverState.hoveringElement = false;
	for(int i = 0; i < _nodeState->nodes.Count(); i++) {
		Rect rect = GetNodeRect(i);
		if(PointInsideRect(mouse, rect)) {
			_nodeEditorState->hoverState.nodeHandle = i;
			_nodeEditorState->hoverState.hoveringElement = true;
		}
	}
}

void ShowNode(int handle)
{
	Node *node = GetNode(_nodeEditorState->draggedNodeHandle);

	glBindFramebuffer(GL_FRAMEBUFFER, _nodeEditorState->fbo);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
    glViewport(0, 0, VIEWER_SIZE, VIEWER_SIZE);

	switch(node->type) {
		case TEXTURE_NODE: {
			int dataHandle = node->GetData();
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
	// START DRAGGING
	if(_nodeEditorState->hoverState.hoveringElement) {
		if(mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] && !_nodeEditorState->isDragging) {
			_nodeEditorState->draggedNodeHandle = _nodeEditorState->hoverState.nodeHandle;
			_nodeEditorState->isDragging = true;
			Node *node = GetNode(_nodeEditorState->hoverState.nodeHandle);
			_nodeEditorState->dragOffset = node->pos - mouse;
		}
	}

	// CONTINUE DRAGGING
	if(_nodeEditorState->isDragging) {

		if(!mouse_buttons[GLFW_MOUSE_BUTTON_LEFT]) {
			// handle stopped dragging
			_nodeEditorState->isDragging = false;
		}
		else {
			// handle continued dragging
			Node *node = GetNode(_nodeEditorState->draggedNodeHandle);
			node->pos = mouse + _nodeEditorState->dragOffset;
		}
	}
}

void NodeGUI()
{
	ImGui::Begin("nodes");
	if(ImGui::Button("add blur node")) {
		AddBlurNode();
	}
	ImGui::End();

	ImGui::Begin("Inspector");
	if(_nodeEditorState->draggedNodeHandle != -1) {
		Node *node = GetNode(_nodeEditorState->draggedNodeHandle);

		ImGui::Text("%s", node->name);


		for(int i = 0; i < node->params.Count(); i++) {

			// EXPOSE
			ImGui::Checkbox("", &node->params[i].exposed);
			ImGui::SameLine();

			char buffer[128];
			sprintf(buffer, "%s", node->params[i].name);

			switch(node->params[i].type) {
				case PARAM_INT: {
					if(ImGui::InputInt(buffer, &node->params[i].i)) {
						node->changed = true;
					}
					break;
				}
				case PARAM_DOUBLE: {
					if(ImGui::InputDouble(buffer, &node->params[i].d)) {
						node->changed = true;
					}
					break;
				}
				case PARAM_VEC3: {
					if(ImGui::InputFloat3(buffer, glm::value_ptr(node->params[i].v3))) {
						node->changed = true;
					}
					break;
				}
				case PARAM_STRING: {
					if(ImGui::InputText(buffer, node->params[i].str, ARRAY_SIZE(node->params[i].str))) {
						node->changed = true;
					}
					break;
				}
			}
		}
	}
	ImGui::End();
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
		ShowNode(_nodeEditorState->draggedNodeHandle);
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
	
	// GUI
	NodeGUI();
}

void CleanupNodeEditor()
{
	free(_nodeEditorState);
}
