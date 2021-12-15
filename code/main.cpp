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
#include "glm/gtc/type_ptr.hpp"

#include "portaudio.h"
#include "stb_vorbis.c"

#define PL_MPEG_IMPLEMENTATION
#include "pl_mpeg.h"

#include "main.h"
#include "imdraw.cpp"
#include "opengl.cpp"
#include "audio.cpp"
#include "util.cpp"
#include "gui.cpp"
#include "glfw_wrapper.cpp"
#include "simplexnoise1234.cpp"
#include "math.cpp"
#include "render.cpp"
#include "vm_array.cpp"
#include "vm_fixedarray.cpp"
#include "string.cpp"
#include "hashmap.cpp"
#include "node/node_parameter.cpp"
#include "node/node.cpp"
#include "node/node_editor.cpp"
#include "scene_editor.cpp"
#include "global_editor.cpp"
#include "opengl_wrapper.cpp"
#include "viewer_render.cpp"
#include "node/texture.cpp"
#include "node/mesh.cpp"
#include "node/nodestate.cpp"
#include "node/save.cpp"
#include "debug.cpp"
// #include "video.cpp"

int main(int argc, char *argv[])
{
	GLFWwindow *win = initGlfw();

	// initialize graphics
	InitializeRender();

	// initialize GUI
	initializeGUI(win);

	// initialize ImDraw
    ImDrawInitialize();

	// Initialize Audio
    if(!audioInitialize()) {
		cleanup();
		exit(1);
	}

	InitializeOpenglWrapper();
	InitializeNodes();
	AddNodeConstructors();
	InitializeNodeEditor();
	InitializeViewerRender();
	InitializeSceneEditor();
	InitializeGlobalEditor();

	if(VIEWER_START_MAIN) {
		SetViewerInMain(true);
	}

    double lastFrame = glfwGetTime();
    while(!glfwWindowShouldClose(win)) {

		///////////////
		// TIME
		///////////////
		double currentFrame = glfwGetTime();
		double deltaTime = (currentFrame - lastFrame);
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
		// bool show = true;
		// ImGui::ShowDemoWindow(&show);


		///////////////
		// RENDER
		///////////////
		UpdateRender();

		///////////////
		// VIEWER RENDERING
		///////////////
		UpdateViewerRender();
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
		// IMDRAW
		///////////////
		glfwMakeContextCurrent(_win);
		int screenWidth, screenHeight;
		GetWindowSize(&screenWidth, &screenHeight);
		glViewport(0, 0, screenWidth, screenHeight);
		ImDrawRender();

		///////////////
		// BUFFER SWAP / IMGUI RENDER
		///////////////
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(win);
    }
    
    cleanup();

    return 0;
}

void cleanup()
{
    audioClose();
    Pa_Terminate();
    imDrawClean();
    glfwTerminate();
	// NodeEditorCleanup();
	// CleanupTextureGraph();
	CleanupNodes();
	CleanupNodeEditor();
	CleanupSceneEditor();
	CleanupGlobalEditor();
}

