#pragma once

typedef glm::vec3 vec3;
typedef glm::vec2 vec2;

#define global static
/* #define SCREEN_WIDTH 1280 */
/* #define SCREEN_HEIGHT 720 */

#define VIEWER_OTHER_WINDOW true
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

/* #define SCREEN_WIDTH 2560 */
/* #define SCREEN_HEIGHT 1440 */

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

bool keys[1024];
bool keys_registered[1024];
vec2 mouse;
/* double mouseX; */
/* double mouseY; */
bool mouse_buttons[32];

#include "audio.h"
#include "opengl.h"
#include "util.h"
/* #include "node_editor.h" */
#include "math.h"
#include "imdraw.h"
#include "render.h"
/* #include "node.h" */
#include "vm_array.h"
#include "node/node.h"
#include "glfw_wrapper.h"
#include "node/node_editor.h"


GLuint createRectVAO(float w, float h);
bool singleKeyPress(int GLFW_KEY);
void cleanup();

// dirty globals
/* global GLuint baseShader; */
/* global GLuint textureShader; */
/* global GLuint cubeVAO; */
