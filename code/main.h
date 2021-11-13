#pragma once

typedef glm::vec3 vec3;
typedef glm::vec2 vec2;

#include "audio.h"
#include "opengl.h"
#include "util.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

const int SCREEN_WIDTH = 1280, SCREEN_HEIGHT = 720;

bool keys[1024];
bool keys_registered[1024];
double mouseX;
double mouseY;
bool mouse_buttons[32];

GLuint createRectVAO(float w, float h);
bool singleKeyPress(int GLFW_KEY);
void cleanup();
