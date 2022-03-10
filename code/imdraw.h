/*
  imdraw.cpp is a 2d immediate mode drawing library
  
  it uses the following coordinate system:

(0, 0)
  -----------------
  |               |                
  |               |
  |               |
  |               |
  |               |
  -----------------
              (WIDTH, HEIGHT)

  USAGE:
  initialize ImDraw before your game loop with:
  ImDrawInitialize();
  and cleanup in the your cleanup phase with:
  ImDrawClean();
  To use imdraw simply call the drawing function you need.
  Fx: 
  ImDrawRect(myRect);
  and you will have a rect on screen.

  if you want to change color of the draw then set the color before drawing with:
  ImDrawSetColor(vec3 col);
  you can also use colors with transparency
  ImDrawSetColor(vec4 colWithAlpha);
  
			  
  LAYERING:
  Each layer is drawn in a seperate draw call. To define which layer you want to draw to
  you need to first call ImDrawSetNextLayer(0 - 2). Right now we are using 3 layers where
  layer 0 is behind and layer 2 is in front. We can easily extend this to more layers.

  
  DEPENDENCIES:
  - opengl
  - stb_truetype.h
  - vm_array.h (my own dynamic array class)
  - glfw_wrapper (my own glfw wrapper, this dependency needs to be removed)

  

  TODO:
  - buffer data every frame
     currently we buffer data every frame, which technically should 
     by allocating every frame. It doesnt seems to have an effect on performance
     so far so maybe opengl is smart enough to reuse the same buffers. 
     it could be a source of optimisation in the future.
  
  - dependency on glfw_wrapper
     we should make sure there is no dependency on the glfw_wrapper
	 


*/

#pragma once

// TODO (rhoe) A stb_truetype implementation is already included from DearImgui
//             If we remove dearimgui we should include stb_truetype ourselves
// #define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation

// #include "stb_truetype.h"
global unsigned char ttf_buffer[1<<20];
global unsigned char temp_bitmap[512*512];
global stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
global GLuint ftex;
global GLuint textShader;

#include "PhormArray.h"

#define FONT_SIZE 18.0
#define MAX_TEXT_SIZE 512
#define IMDRAW_VBO_BUFFER_SIZE 1024

struct TextCommand {
	char text[MAX_TEXT_SIZE];
	vec2 pos;
	vec3 color;
};

#define IMDRAW_LAYER_AMOUNT 3

struct ImDrawLayer {
	PArray<GLfloat> vertices;
};

global ImDrawLayer imDrawLayers[IMDRAW_LAYER_AMOUNT];
// global PArray<GLfloat> imDrawVertices;
global GLuint imDrawVAO;
global GLuint imDrawVBO;
global GLuint imDrawShader;
global vec4 imDrawColor;
global int imDrawNextLayer;
global PArray<TextCommand> imTextCommands;
// global size_t imVBOSize;

global GLuint imdrawTextVAO;
global GLuint imdrawTextVBO;
global GLuint imdrawTextEBO;


void ImDrawSetColor(vec3 color);
void ImDrawPushQuad(vec2 p1, vec2 p2, vec2 p3, vec2 p4);
void ImDrawPushTriangle(vec2 p1, vec2 p2, vec2 p3);
void ImDrawPoint(vec2 p, float size = 1.0f);
void ImDrawLine(vec2 a, vec2 b, float size = 1.0f);
void ImDrawRectOutline(Rect rect, float size = 1.0f);
void ImDrawRect(Rect rect);
void ImDrawText(vec2 pos, char *text);
void ImDrawText(vec2 pos, char *text, vec3 color);
