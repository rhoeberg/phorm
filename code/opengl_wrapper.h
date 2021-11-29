#pragma once

struct OpenglContextState {
	VMArray<GLuint> VAOObjects;
};

struct OpenglWrapperState {
	// OpenglContextState mainWindowContext;
	// OpenglContextState viewerWindowContext;

	VMArray<GLuint> mainContextVAO;
	VMArray<GLuint> viewerContextVAO;

	bool viewerInMain;
};

void InitializeOpenglWrapper();
int AddVAOMainWindow();
int AddVAOViewerWindow();
int AddVAO();
void ToggleViewer();
void SetViewerInMain(bool value);
bool ViewerInMain();
GLuint GetMainContextVAO(int handle);
GLuint GetViewerContextVAO(int handle);
GLuint GetCurrentContextVAO(int handle);
void BindBuffersToVAO(int VAOHandle, GLuint VBO, GLuint EBO);
/* void BindBufferToVAO(int handle, int type, GLuint bufferID); */
/* void SetVAOAttribPointers(int handle); */
int CreateSpriteVAO();
void CleanupOpenglWrapper();

global OpenglWrapperState *_openglWrapperState;
