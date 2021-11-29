/*
  
  we can abstract the VAO creation by using handles to VAO's instead of direct
  VAO id's.
  This way we can manage 2 opengl context at the same time by keeping the VAO handles 
  synced between the 2 context and then just returning the "real" VAO depending on 
  what context is currently used.

 */

void InitializeOpenglWrapper()
{
	_openglWrapperState = (OpenglWrapperState*)malloc(sizeof(OpenglWrapperState));
	_openglWrapperState->mainContextVAO = VMArray<GLuint>();
	_openglWrapperState->viewerContextVAO = VMArray<GLuint>();

	_openglWrapperState->viewerInMain = true;
}

void ToggleViewer()
{
	SetViewerInMain(!_openglWrapperState->viewerInMain);
}

void SetViewerInMain(bool value)
{
	_openglWrapperState->viewerInMain = value;
	if(value) {
		HideViewerOtherWindow();
	}
	else {
		ShowViewerOtherWindow();
	}
}

int AddVAOMainWindow()
{
    glfwMakeContextCurrent(_win);
	GLuint VAO;
    glGenVertexArrays(1, &VAO);
	int result = _openglWrapperState->mainContextVAO.Insert(VAO);

	return result;
}

int AddVAOViewerWindow()
{
    glfwMakeContextCurrent(_viewerWindow);
	GLuint VAO;
    glGenVertexArrays(1, &VAO);
	int result = _openglWrapperState->viewerContextVAO.Insert(VAO);

	return result;
}

int AddVAO()
{
	int mainResult = AddVAOMainWindow();
	int viewerResult = AddVAOViewerWindow();

	ASSERT(viewerResult == mainResult);

	return mainResult;
}

bool ViewerInMain()
{
	return _openglWrapperState->viewerInMain;
}

GLuint GetMainContextVAO(int handle)
{
	return _openglWrapperState->mainContextVAO[handle];
}

GLuint GetViewerContextVAO(int handle)
{
	return _openglWrapperState->viewerContextVAO[handle];
}

GLuint GetCurrentContextVAO(int handle)
{
	if(ViewerInMain()) {
		return GetMainContextVAO(handle);
	}
	else {
		return GetViewerContextVAO(handle);
	}
}

void BindBuffersToVAO(int VAOHandle, GLuint VBO, GLuint EBO)
{
    glfwMakeContextCurrent(_win);
	glBindVertexArray(GetMainContextVAO(VAOHandle));
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glfwMakeContextCurrent(_viewerWindow);
	glBindVertexArray(GetViewerContextVAO(VAOHandle));
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

int CreateSpriteVAO()
{
    GLfloat vertices[] = {
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
    };

    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	int VAOHandle = AddVAO();
	BindBuffersToVAO(VAOHandle, VBO, EBO);
	BindBuffersToVAO(VAOHandle, VBO, EBO);

    return VAOHandle;
}

void CleanupOpenglWrapper()
{
	_openglWrapperState->mainContextVAO.Free();
	_openglWrapperState->viewerContextVAO.Free();
	free(_openglWrapperState);
}
