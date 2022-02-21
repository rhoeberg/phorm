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
	new(&_openglWrapperState->mainContextVAO) VMArray<GLuint>();
	new(&_openglWrapperState->viewerContextVAO) VMArray<GLuint>();
	new(&_openglWrapperState->buffers) VMArray<Buffer>();
	new(&_openglWrapperState->textures) VMArray<GFXTexture>();

	_openglWrapperState->viewerInMain = true;

	_openglWrapperState->defaultTexture = GFXAddTexture();

	// create default texture
	// glGenTextures(1, &defaultTexture);

	Pixel white = Pixel(255, 255, 255, 255);
	Bitmap whiteBitmap(1, 1, &white);

	GFXBindTexture(_openglWrapperState->defaultTexture);
	// glBindTexture(GL_TEXTURE_2D, defaultTexture);
	GFXTextureUploadBitmap(_openglWrapperState->defaultTexture, &whiteBitmap);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &white);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glBindTexture(GL_TEXTURE_2D, 0);


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


i32 AddStorageBuffer(u64 size)
{
	Buffer result;
	glGenBuffers(1, &result.id);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, result.id);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_DYNAMIC_READ);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	return _openglWrapperState->buffers.Insert(result);
}

bool ViewerInMain()
{
	return _openglWrapperState->viewerInMain;
}

GLuint GetMainContextVAO(int handle)
{
	return _openglWrapperState->mainContextVAO[handle];
}

void BindMainContextVAO(int handle)
{
	glBindVertexArray(_openglWrapperState->mainContextVAO[handle]);
}

void BindViewerContextVAO(int handle)
{
	glBindVertexArray(_openglWrapperState->viewerContextVAO[handle]);
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

void SetContextMain()
{
	glfwMakeContextCurrent(_win);
}

void SetContextViewer()
{
	glfwMakeContextCurrent(_viewerWindow);
}

void BindBuffersToVAO(int VAOHandle, GLuint VBO, GLuint EBO)
{
    glfwMakeContextCurrent(_win);
	glBindVertexArray(GetMainContextVAO(VAOHandle));
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindVertexArray(0);

    glfwMakeContextCurrent(_viewerWindow);
	glBindVertexArray(GetViewerContextVAO(VAOHandle));
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindVertexArray(0);
}

int CreateSpriteVAO()
{
    GLfloat vertices[] = {
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
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

GLuint CreateShader(const char *path, int type)
{
    const char* source = readFile(path);
    GLint success;
    GLchar infoLog[512];
    GLuint id;
    id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(!success) {
		glGetShaderInfoLog(id, 512, NULL, infoLog);
        printf("%s\n", infoLog);
    }
	return id;
}

GLuint CreateShaderProgram(const char *vPath, const char *fPath)
{
    GLuint shaderProgram;
    const char* vertexCode = readFile(vPath);
    const char* fragmentCode = readFile(fPath);

    GLint success;
    GLchar infoLog[512];
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
        printf("%s\n", infoLog);
    }
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
        printf("%s\n", infoLog);
    }
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

///////////////
// GLOBAL
///////////////
void GFXClear(vec3 color)
{
	glClearColor(color.r, color.g, color.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
}

void GFXSetViewport(i32 x, i32 y, i32 width, i32 height)
{
	glViewport(x, y, width, height);
}


///////////////
// RESOURCES
///////////////


// TEXTURES
i32 GFXAddTexture()
{
	GFXTexture result;
	glGenTextures(1, &result.id);
	glBindTexture(GL_TEXTURE_2D, result.id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return _openglWrapperState->textures.Insert(result);
}

void GFXTextureUploadBitmap(i32 handle, Bitmap *bitmap)
{
	GFXTexture *texture = &_openglWrapperState->textures[handle];
	
	glBindTexture(GL_TEXTURE_2D, texture->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap->width, bitmap->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GFXBindTexture(i32 handle)
{
	GFXTexture *texture = &_openglWrapperState->textures[handle];
	glBindTexture(GL_TEXTURE_2D, texture->id);
}

void GFXBindDefaultTexture()
{
	GFXBindTexture(_openglWrapperState->defaultTexture);
}

void GFXUnbindTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CleanupOpenglWrapper()
{
	_openglWrapperState->mainContextVAO.Free();
	_openglWrapperState->viewerContextVAO.Free();
	free(_openglWrapperState);
}
