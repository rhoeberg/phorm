// LIBRARIES
#include <iostream>
#include <math.h>

// #define GLEW_STATIC
#include "GL/glew.h"

#define GLFW_DLL
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM

#include "imgui/imgui.cpp"
#include "imgui/imgui_draw.cpp"
#include "imgui/imgui_impl_glfw.cpp"
#include "imgui/imgui_impl_opengl3.cpp"
#include "imgui/imgui_widgets.cpp"
#include "imgui/imgui_tables.cpp"
#include "imgui/imgui_demo.cpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/vector_angle.hpp"

#include "portaudio.h"
#include "stb_vorbis.c"

#define PL_MPEG_IMPLEMENTATION
#include "pl_mpeg.h"

#include "main.h"
#include "imdraw.cpp"
#include "imdraw3D.cpp"
#include "opengl.cpp"
#include "audio.cpp"
#include "util.cpp"
#include "gui.cpp"
#include "glfw_wrapper.cpp"
#include "simplexnoise1234.cpp"
#include "math.cpp"
// #include "render.cpp"
#include "vm_array.cpp"
#include "vm_fixedarray.cpp"
#include "string.cpp"
#include "hashmap.cpp"
#include "ObjectContainer.cpp"
#include "NodeConstructor.cpp"
#include "node_parameter.cpp"
#include "node.cpp"
#include "node_editor.cpp"
#include "scene_editor.cpp"
#include "global_editor.cpp"
#include "opengl_wrapper.cpp"
#include "viewer_render.cpp"
#include "texture.cpp"
#include "mesh.cpp"
#include "data.cpp"
#include "save.cpp"
#include "debug.cpp"

// Compile custom user code
#include "../custom/CustomCompile.h"
#include "../custom/CustomInit.cpp"

void UpdateLoop()
{

	///////////////
	// TIME
	///////////////
	frameCount++;
	double currentFrame = glfwGetTime();
	deltaTime = (currentFrame - lastFrame);
	lastFrame = currentFrame;

	///////////////
	// INPUT
	///////////////
	glfwPollEvents();
	
	///////////////
	// GUI
	///////////////
	glfwMakeContextCurrent(_win);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	// gui();
	bool show = true;
	ImGui::ShowDemoWindow(&show);

	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

	///////////////
	// VIEWER RENDERING
	///////////////
	UpdateViewerRenderGUI();

	///////////////
	// GLOBAL EDITOR
	///////////////
	UpdateGlobalEditor();

	///////////////
	// DEBUG
	///////////////
	// TODO (rhoe) should maybe be switched off by compiler macro
	UpdateDebug();

	///////////////
	// RENDERING
	///////////////
	UpdateViewerRender();

	///////////////
	// IMDRAW
	///////////////
	SetContextMain();
	int screenWidth, screenHeight;
	GetWindowSize(&screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);
	ImDrawRender();

	///////////////
	// BUFFER SWAP / IMGUI RENDER
	///////////////
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(_win);
}

int main(int argc, char *argv[])
{
	///////////
	// INITIALIZATION
	///////////
	GLFWwindow *win = initGlfw();

	// initialize GUI
	initializeGUI(win);

	InitializeOpenglWrapper();

	// initialize ImDraw
    ImDrawInitialize();
	ImDraw3DInitialize();

	// Initialize Audio
    if(!audioInitialize()) {
		cleanup();
		exit(1);
	}

	// Initialize Application
	InitializeData();
	InitializeNodeConstructors();

	// add user nodes to system
	CustomInit();

	InitializeNodeEditor();
	InitializeViewerRender();
	InitializeSceneEditor();
	InitializeGlobalEditor();



	///////////
	// LOAD SETTINGS
	///////////

	// load default scene
	if(argc > 1) {
		String arg1 = argv[1];
		if(arg1.Equals("load")) {
			printf("loading scene on start\n");
			LoadNodes();
		}
	}

	// load viewer settings
	SetViewerInMain(VIEWER_START_MAIN);

	// load windows settings
	glfwSettings.mainWindow.posX = 100;
	glfwSettings.mainWindow.posY = 100;
	glfwSettings.viewerWindow.posX = 2600;
	glfwSettings.viewerWindow.posY = 100;
	SetWindowSettings(_win, glfwSettings.mainWindow);
	SetWindowSettings(_viewerWindow, glfwSettings.viewerWindow);


	////////////
	// UPDATE LOOP
	////////////
    lastFrame = glfwGetTime();
	frameCount = 0;
	double startTime = 0.0;
    while(!glfwWindowShouldClose(win)) {
		UpdateLoop();
    }
    

	////////////
	// DONE
	////////////
    cleanup();
    return 0;
}

void cleanup()
{
    audioClose();
    Pa_Terminate();
    imDrawClean();
    glfwTerminate();
	CleanupNodes();
	CleanupCustom();
	CleanupNodeEditor();
	CleanupSceneEditor();
	CleanupGlobalEditor();
}

