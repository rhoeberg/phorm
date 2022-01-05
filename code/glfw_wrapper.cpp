global GLFWwindow *_win;
global GLFWwindow *_viewerWindow;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if(key > 0) {
		ImGuiIO& io = ImGui::GetIO();
		imguiWantsKeyboard = io.WantCaptureKeyboard;

		if(action == GLFW_PRESS) {
			keys[key] = true;
		}
		else if(action == GLFW_RELEASE) {
			keys[key] = false;
		}
	}
}  

void SetMouse(double x, double y)
{
	mouse.x = x;
	mouse.y = y;
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	ImGuiIO& io = ImGui::GetIO();
	imguiWantsMouse = io.WantCaptureMouse;
	mouseInViewerWin = false;
	SetMouse(xpos, ypos);
}

void mouseCallbackViewerWin(GLFWwindow* window, double xpos, double ypos)
{
	mouseInViewerWin = true;
	SetMouse(xpos, ypos);
	mouse.x = xpos;
	mouse.y = ypos;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	ImGuiIO& io = ImGui::GetIO();
	if(!io.WantCaptureMouse) {

		if(action == GLFW_PRESS) {
			mouse_buttons[button] = true;
		}
		else if(action == GLFW_RELEASE) {
			mouse_buttons[button] = false;
		}
	}
}

void MouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	scrollOffset = yoffset;
	scrollReady = true;
}

static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

bool singleKeyPress(int GLFW_KEY)
{
    if(keys[GLFW_KEY] && !keys_registered[GLFW_KEY]) {
        keys_registered[GLFW_KEY] = true;
        return true;
    } 
    else if(!keys[GLFW_KEY] && keys_registered[GLFW_KEY]) {
        keys_registered[GLFW_KEY] = false;
        return false;
    }
    else {
        return false;
    }
}

void dropCallback(GLFWwindow *window, int count, const char **paths)
{
	int i;
	for(i = 0; i < count; i++) {
		// printf("dropped file:%s\n", paths[i]);
		sprintf(droppedFile, "%s", paths[i]);
		readyToConsumeDropped = true;
	}
}

void windowSizeCallback(GLFWwindow *window, int width, int height)
{
	// TODO (rhoe) remove coupling with application code
	ImDrawSetWindowSize(width, height);
	// NodeEditorSetWindowSize(width, height);
}

void ViewerWindowSizeCallback(GLFWwindow *window, int width, int height)
{
	// TODO (rhoe) remove coupling with application code
}

void GetViewerWindowSize(int *width, int *height)
{
	glfwGetWindowSize(_viewerWindow, width, height);
}

void GetWindowSize(int *width, int *height)
{
	glfwGetWindowSize(_win, width, height);
}

void ShowViewerOtherWindow()
{
	glfwShowWindow(_viewerWindow);
}

void HideViewerOtherWindow()
{
	glfwHideWindow(_viewerWindow);
}

double GetTime()
{
	return glfwGetTime() - startTime;
}

void ResetTime()
{
	startTime = glfwGetTime();
}

void SetWindowSettings(GLFWwindow *win, WindowSettings settings)
{
	glfwSetWindowPos(win, settings.posX, settings.posY);
}

GLFWwindow* initGlfw()
{
	startTime = 0.0;
	// GLFWwindow *win;

    glfwSetErrorCallback(errorCallback);
    if(!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);


	/////////////////////
	// CREATE MAIN WINDOW
    _win = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "test", NULL, NULL);
    if(!_win) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(_win);

    glfwSetCursorPosCallback(_win, mouseCallback);
    glfwSetKeyCallback(_win, keyCallback);
    glfwSetMouseButtonCallback(_win, mouseButtonCallback);
    glfwSetScrollCallback(_win, MouseScrollCallback);
    glfwSetDropCallback(_win, dropCallback);
    glfwSetWindowSizeCallback(_win, windowSizeCallback);
    int vWidth, vHeight;
    glfwGetFramebufferSize(_win, &vWidth, &vHeight);
    glViewport(0, 0, vWidth, vHeight);
    // enable depths testing to remove pixels which is behind other pixels
    glEnable(GL_DEPTH_TEST);  

	/////////////////////
	// CREATE VIEWER WINDOW
	if(VIEWER_START_MAIN)
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    _viewerWindow = glfwCreateWindow(VIEWER_WIDTH, VIEWER_HEIGHT, "test", NULL, _win);
    if(!_viewerWindow) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(_viewerWindow);

	// TODO (rhoe) make callbacks for the viewer?
    glfwSetCursorPosCallback(_viewerWindow, mouseCallbackViewerWin);
    glfwSetKeyCallback(_viewerWindow, keyCallback);
    glfwSetMouseButtonCallback(_viewerWindow, mouseButtonCallback);
    glfwSetScrollCallback(_viewerWindow, MouseScrollCallback);
    // glfwSetDropCallback(_win, dropCallback);
    glfwSetWindowSizeCallback(_win, ViewerWindowSizeCallback);
    glfwGetFramebufferSize(_viewerWindow, &vWidth, &vHeight);
    glViewport(0, 0, vWidth, vHeight);
    glEnable(GL_DEPTH_TEST);  
    glfwMakeContextCurrent(_win);

    // Set this to true so glew knows to use modern opengl
    glewExperimental = GL_TRUE;
    glewInit();

	return _win;
}
