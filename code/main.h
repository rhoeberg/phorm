#pragma once

#define global static
#define VIEWER_START_MAIN false
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define VIEWER_WIDTH 1920
#define VIEWER_HEIGHT 1080
/* #define SCREEN_WIDTH 2560 */
/* #define SCREEN_HEIGHT 1440 */

#include "types.h"

global bool keys[1024];
global bool keys_registered[1024];
global vec2 mouse;
global bool mouse_buttons[32];
global bool mouseInViewerWin;
global bool scrollReady;
global double scrollOffset;
global bool imguiWantsKeyboard;
global bool imguiWantsMouse;
global double deltaTime;
global double lastFrame;
global u64 frameCount;
global GLFWSettings glfwSettings;

#include "audio.h"
#include "opengl.h"
#include "util.h"
#include "simplexnoise1234.h"
#include "math.h"
/* #include "render.h" */
#include "vm_array.h"
#include "vm_fixedarray.h"
#include "imdraw.h"
#include "Imdraw3D.h"
#include "string.h"
#include "hashmap.h"
#include "ObjectContainer.h"
#include "glfw_wrapper.h"
#include "opengl_wrapper.h"
#include "light.h"
#include "node_parameter.h"
#include "node.h"
#include "data.h"
#include "node_editor.h"
#include "nodes/node_op.h"
#include "global_editor.h"
#include "scene.h"
#include "scene_editor.h"
#include "viewer_render.h"
#include "save.h"
#include "Camera.h"

/* #include "node/texture.h" */
/* #include "node/mesh.h" */

GLuint createRectVAO(float w, float h);
bool singleKeyPress(int GLFW_KEY);
void cleanup();
