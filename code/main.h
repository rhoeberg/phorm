#pragma once

typedef glm::vec4 vec4;
typedef glm::vec3 vec3;
typedef glm::vec2 vec2;

#define global static
#define VIEWER_START_MAIN true
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
/* #define SCREEN_WIDTH 2560 */
/* #define SCREEN_HEIGHT 1440 */

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#define ASSERT(Expression) if(!(Expression)) {*(volatile int *)0 = 0;}
/* #define ASSERT(expr) static_assert(expr, "Assertion failed: " #expr) */
#define NOT_IMPLEMENTED ASSERT(!"NOT_IMPLEMENTED")

global bool keys[1024];
global bool keys_registered[1024];
global vec2 mouse;
global bool mouse_buttons[32];
global bool mouseInViewer;
global bool scrollReady;
global double scrollOffset;
global bool imguiWantsKeyboard;
global bool imguiWantsMouse;

#include "audio.h"
#include "opengl.h"
#include "util.h"
#include "simplexnoise1234.h"
#include "types.h"
#include "math.h"
#include "imdraw.h"
#include "render.h"
#include "vm_array.h"
#include "vm_fixedarray.h"
#include "string.h"
#include "hashmap.h"
#include "glfw_wrapper.h"
#include "opengl_wrapper.h"
#include "light.h"
#include "node/node_parameter.h"
#include "node/node.h"
#include "node/nodestate.h"
#include "node/node_editor.h"
#include "node/node_op.h"
#include "global_editor.h"
#include "scene.h"
#include "scene_editor.h"
#include "viewer_render.h"
#include "node/save.h"
/* #include "node/texture.h" */
/* #include "node/mesh.h" */

GLuint createRectVAO(float w, float h);
bool singleKeyPress(int GLFW_KEY);
void cleanup();
