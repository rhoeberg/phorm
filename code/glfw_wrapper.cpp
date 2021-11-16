void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, GL_TRUE);

    if(action == GLFW_PRESS) {
        keys[key] = true;
    }
    else if(action == GLFW_RELEASE) {
        keys[key] = false;
    }
}  

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	mouse.x = xpos;
	mouse.y = ypos;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if(action == GLFW_PRESS) {
		mouse_buttons[button] = true;
    }
    else if(action == GLFW_RELEASE) {
		mouse_buttons[button] = false;
    }
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
		printf("dropped file:%s\n", paths[i]);
	}
}

GLFWwindow* initGlfw()
{
	GLFWwindow *win;

    glfwSetErrorCallback(errorCallback);
    if(!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    win = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "test", NULL, NULL);
    if(!win) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(win);
    
    glfwSetCursorPosCallback(win, mouseCallback);
    glfwSetKeyCallback(win, keyCallback);
    glfwSetMouseButtonCallback(win, mouseButtonCallback);
    glfwSetDropCallback(win, dropCallback);

    // Set this to true so glew knows to use modern opengl
    glewExperimental = GL_TRUE;
    glewInit();
    
    int vWidth, vHeight;
    glfwGetFramebufferSize(win, &vWidth, &vHeight);
    glViewport(0, 0, vWidth, vHeight);
    
    // enable depths testing to remove pixels which is behind other pixels
    glEnable(GL_DEPTH_TEST);  

	return win;
}
