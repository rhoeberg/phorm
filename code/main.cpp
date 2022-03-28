// LIBRARIES
#include <iostream>
#include <math.h>

// #define GLEW_STATIC
#include "GL/glew.h"

#ifdef WIN
#define GLFW_DLL
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

/*#ifdef VULKAN
#define GLFW_INCLUDE_VULKAN
#endif*/

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

// #include "portaudio.h"
// #include "stb_vorbis.c"

#ifdef WIN
#define __WINDOWS_MM__
#endif

#include "rtmidi/RtMidi.h"
#include "rtmidi/RtMidi.cpp"

#define PL_MPEG_IMPLEMENTATION
#include "pl_mpeg.h"

#include "tinyfiledialogs.h"
#include "tinyfiledialogs.c"

#include "main.h"
#include "imdraw.cpp"
#include "ImDraw3D.cpp"
// #include "opengl.cpp"
// #include "audio.cpp"
#include "PhormUtil.cpp"
#include "gui.cpp"
#include "glfw_wrapper.cpp"
#include "simplexnoise1234.cpp"
#include "math.cpp"
#include "PhormArray.cpp"
#include "vm_fixedarray.cpp"
#include "string.cpp"
#include "PhormHashmap.cpp"
#include "ObjectContainer.cpp"
#include "NodeConstructor.cpp"
#include "midi.cpp"
#include "PhormNetwork.cpp"
#include "PhormOSC.cpp"
#include "PhormProject.cpp"
#include "node_parameter.cpp"
#include "node.cpp"
#include "node_editor.cpp"
#include "scene_editor.cpp"
#include "PhormSceneRender.cpp"
#include "global_editor.cpp"
#include "PhormOpengl.cpp"
#include "ViewerRender.cpp"
#include "scene.cpp"
#include "PhormRenderObject.cpp"
#include "Bitmap.cpp"
#include "mesh.cpp"
#include "data.cpp"
#include "save.cpp"

// GFX LAYER
/*#ifdef VULKAN
#include "VUlkanLayer.cpp"
#endif*/

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

	// bool show = true;
	// ImGui::ShowDemoWindow(&show);

	GFXClear(COLOR_BG);

	///////////////
	// OSC
	///////////////
	// UpdateOSC();

	///////////////
	// VIEWER RENDERING
	///////////////
	// UpdateViewerRenderGUI();

	///////////////
	// GLOBAL EDITOR
	///////////////
	UpdateGlobalEditor();

	////////////////
	// NODES
	////////////////
	UpdateNodes();

	///////////////
	// RENDERING
	///////////////
	if(!FullscreenViewer()) {
		///////////////
		// IMDRAW
		///////////////
		SetContextMain();
		int screenWidth, screenHeight;
		// GetWindowSize(&screenWidth, &screenHeight);
		GetFramebufferSize(&screenWidth, &screenHeight);
		glViewport(0, 0, screenWidth, screenHeight);
		ImDrawRender();
	}

	SetContextViewer();
	UpdateSceneRender();
	UpdateViewerRender();

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

	// initialize GFX Backend
	//GFXLayerInit();

	// initialize GUI
	initializeGUI(win);

	InitializeOpenglWrapper();

	// initialize ImDraw
    ImDrawInitialize();
	ImDraw3DInitialize();


	// Initialize Audio
    // if(!audioInitialize()) {
    //     	cleanup();
    //     	exit(1);
    //     }


	// Initialize Application
	InitializeData();
	InitializeGlobalEditor();
	InitializeNetwork();
	InitializeNodeConstructors();
	InitializeMidi();

	// add user nodes to system
	CustomInit();

	InitializeOSC();
	InitializeNodeEditor();
	InitializeViewerRender();
	InitializeSceneEditor();
	InitializeSceneRender();
	InitializeProjectState();
	

	// TODO (rhoe) find place for this
    srand(glfwGetTime());

	///////////
	// LOAD SETTINGS
	///////////

	// Try to load arg1 as scene path
	if(argc > 1) {
		String arg1 = argv[1];
		ProjectLoad(arg1);
	}

	// load last settings
	bool success = LoadSettings();
	if(!success) {
		// if no last settings load default settings
		settings.mainWindow.posX = 100;
		settings.mainWindow.posY = 100;
		settings.mainWindow.width = 1920;
		settings.mainWindow.height = 1080;

		settings.viewerWindow.posX = 2600;
		settings.viewerWindow.posY = 100;
		settings.viewerWindow.width = 1920;
		settings.viewerWindow.height = 1080;

		settings.viewerInMain = false;
	}
	// settings.viewerInMain = true;
	SetWindowSettings(settings);

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

	// save last settings
	SaveSettings();

    cleanup();
    return 0;
}

void cleanup()
{
    // audioClose();
    // Pa_Terminate();

    imDrawClean();
    glfwTerminate();
	CleanupMidi();
	CleanupNetwork();
	CleanupNodes();
	CleanupCustom();
	CleanupNodeEditor();

	CleanupSceneEditor();
	CleanupSceneRender();

	CleanupViewerRender();
	CleanupGlobalEditor();
	CleanupProjectState();
	CleanupData();
}

