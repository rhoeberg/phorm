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

#include "main.h"
#include "imdraw.cpp"
#include "gui.cpp"
#include "opengl.cpp"
#include "audio.cpp"
#include "util.cpp"

int main(int argc, char *argv[])
{
	GLFWwindow *win = initGlfw();

	// initialize graphics
    GLuint shaderProgram = createShaderProgram("assets/vertexshader.vs", "assets/fragmentshader.frag");
    glUseProgram(shaderProgram);
    glm::mat4 view = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
    glm::mat4 projection;
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	initializeGUI(win);
    imDrawInitialize(view, projection);

    bool success = audioInitialize(&mixer);
    if(!success) {
		cleanup();
		exit(1);
    }

	GLuint cube = createCubeVAO();

    double lastFrame = glfwGetTime();
    while(!glfwWindowShouldClose(win)) {

		// TIME
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
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		gui();
		// bool show = true;
		// ImGui::ShowDemoWindow(&show);

		///////////////
		//RENDER
		///////////////
		glClearColor(0.5f, 0.05f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

		glUseProgram(shaderProgram);

		// render cube
		glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
		model = glm::translate(model, glm::vec3(0, 0, 0));
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0, 1, 0));
		GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(cube);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

		glUseProgram(0);

		imDrawRender();
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
}
