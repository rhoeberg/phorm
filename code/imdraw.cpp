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

			  
  LAYERING:
  currently we are drawing one mesh for all the basic shapes and a single mesh pr font
  character. If want to be able to control layer order we need to do it differently.
  we need to draw our shapes in the order we want to layer them back to front.
  this means that we need to keep a list of shapes and rearrange the order whenever we
  add new shapes.

  - simple solution with layers
  Simplest would be to just add draw data as we do now (just call ImDrawRect whenever you want to
  etc), and then add extra layer data to the call. 
  every frame we can then sort through all the draw data and make sure we draw in the right order.
  this is super simple to implement but is obviously very slow.
  
  - even more simple solution (just 2 or 3 layers)
  even more simple would be to keep the basic drawing functionality as it is now, the only thing
  we change is just to add different vertex data lists for each layer and just add a limited 
  amount of layers. When you do a draw call you just specify which layer you want to draw to.
  this means that two objects on the same layer wont blend nicely but at least objects on differnt
  layers should be fine.
  this might be enough for us right now.
  
  - more complete solution:
  alternativly we need to somehow keep track of objects and make sure only to add them once which
  either makes the library much more inconvienient for the user (the user has to manage their
  drawing objects themselves) or we need to do some kind of id's for the objects which the drawing
  library keeps track of.
  
  


			  
  TODO:
  
  - buffer data every frame
     currently we buffer data every frame, which technically should 
     by allocating every frame. It doesnt seems to have an effect on performance
     so far so maybe opengl is smart enough to reuse the same buffers. 
     it could be a source of optimisation in the future.
  
  - dependency on glfw_wrapper
     we should make sure there is no dependency on the glfw_wrapper
	 


 */

// TODO (rhoe) A stb_truetype implementation is already included from DearImgui
//             If we remove dearimgui we should include stb_truetype ourselves
// #define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
// #include "stb_truetype.h"
unsigned char ttf_buffer[1<<20];
unsigned char temp_bitmap[512*512];
stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
GLuint ftex;
GLuint textShader;

#include "vm_array.h"

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
	VMArray<GLfloat> vertices;
};

global ImDrawLayer imDrawLayers[IMDRAW_LAYER_AMOUNT];
// global VMArray<GLfloat> imDrawVertices;
global GLuint imDrawVAO;
global GLuint imDrawVBO;
global GLuint imDrawShader;
global vec4 imDrawColor;
global int imDrawNextLayer;
global VMArray<TextCommand> imTextCommands;
// global size_t imVBOSize;

global GLuint imdrawTextVAO;
global GLuint imdrawTextVBO;
global GLuint imdrawTextEBO;

void ImDrawSetWindowSize(int width, int height)
{
    glm::mat4 projection;
    projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
	glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
    model = glm::translate(model, glm::vec3(0,0,0));
	glm::mat4 view = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
    view = glm::translate(view, glm::vec3(0,0,-3.0f));

    glUseProgram(imDrawShader);
    GLuint projectionLoc = glGetUniformLocation(imDrawShader, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    GLuint modelLoc = glGetUniformLocation(imDrawShader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    GLuint viewLoc = glGetUniformLocation(imDrawShader, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	glUseProgram(textShader);
    projectionLoc = glGetUniformLocation(textShader, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    modelLoc = glGetUniformLocation(imDrawShader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void ImDrawInitialize()
{
    imDrawShader = createShaderProgram("assets\\imdraw.vs", "assets\\imdraw.frag");
	textShader = createShaderProgram("assets/font_shader.vert", "assets/font_shader.frag");

    glGenVertexArrays(1, &imDrawVAO);
    glGenBuffers(1, &imDrawVBO);

	glUseProgram(imDrawShader);
	glBindVertexArray(imDrawVAO);

	// start by allocation a default size on gpu
	glBindBuffer(GL_ARRAY_BUFFER, imDrawVBO);
	// imVBOSize = sizeof(GLfloat) * IMDRAW_VBO_BUFFER_SIZE;
	// glBufferData(GL_ARRAY_BUFFER, imVBOSize, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	int width, height;
	GetWindowSize(&width, &height);
	ImDrawSetWindowSize(width, height);

	// initialize layers
	imDrawNextLayer = 2;
	for(int i = 0; i < ARRAY_SIZE(imDrawLayers); i++) {
		imDrawLayers[i].vertices = VMArray<GLfloat>();
	}

	/////////////////////////
	// INITIALIZE FONT DRAWING
	FILE *file = fopen("c:/windows/fonts/consola.ttf", "rb");
	if(!file) {
		printf("font file not found\n");
	}
	fread(ttf_buffer, 1, 1<<20, file);
	stbtt_BakeFontBitmap(ttf_buffer,0, FONT_SIZE, temp_bitmap,512,512, 32,96, cdata); // no guarantee this fits!
	// can free ttf_buffer at this point

	glGenTextures(1, &ftex);
	glBindTexture(GL_TEXTURE_2D, ftex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512,512, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
	// can free temp_bitmap at this point
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glGenVertexArrays(1, &imdrawTextVAO);
	glGenBuffers(1, &imdrawTextVBO);
	glGenBuffers(1, &imdrawTextEBO);

	glBindVertexArray(imdrawTextVAO);
	glBindBuffer(GL_ARRAY_BUFFER, imdrawTextVBO);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void ImDrawSetNextLayer(int layer)
{
	if(layer < 0 || layer > ARRAY_SIZE(imDrawLayers))
		return;

	imDrawNextLayer = layer;
}

void ImDrawSetColor(vec3 color)
{
	imDrawColor = vec4(color, 1.0f);
}

void ImDrawSetColor(vec4 color)
{
	imDrawColor = color;
}

void ImDrawAddVertex(vec2 v)
{
	// imDrawVertices.Insert(v.x);
	// imDrawVertices.Insert(v.y);
	// imDrawVertices.Insert(imDrawColor.r);
	// imDrawVertices.Insert(imDrawColor.g);
	// imDrawVertices.Insert(imDrawColor.b);
	// imDrawVertices.Insert(imDrawColor.a);

	imDrawLayers[imDrawNextLayer].vertices.Insert(v.x);
	imDrawLayers[imDrawNextLayer].vertices.Insert(v.y);
	imDrawLayers[imDrawNextLayer].vertices.Insert(imDrawColor.r);
	imDrawLayers[imDrawNextLayer].vertices.Insert(imDrawColor.g);
	imDrawLayers[imDrawNextLayer].vertices.Insert(imDrawColor.b);
	imDrawLayers[imDrawNextLayer].vertices.Insert(imDrawColor.a);
}

void ImDrawPushQuad(vec2 p1, vec2 p2, vec2 p3, vec2 p4)
{
    // triangle 1
    ImDrawAddVertex(p1);
    ImDrawAddVertex(p2);
    ImDrawAddVertex(p3);
    // triangle 2
    ImDrawAddVertex(p3);
    ImDrawAddVertex(p4);
    ImDrawAddVertex(p1);
}

void ImDrawPushTriangle(vec2 p1, vec2 p2, vec2 p3)
{
    ImDrawAddVertex(p1);
    ImDrawAddVertex(p2);
    ImDrawAddVertex(p3);
}

void ImDrawPoint(vec2 p, float size)
{
	float halfSize = size * 0.5f;
    vec2 p1 = vec2(p.x - halfSize, p.y + halfSize);
    vec2 p2 = vec2(p.x + halfSize, p.y + halfSize);
    vec2 p3 = vec2(p.x + halfSize, p.y - halfSize);
    vec2 p4 = vec2(p.x - halfSize, p.y - halfSize);
    ImDrawPushQuad(p1, p2, p3, p4);
}


void ImDrawLine(vec2 a, vec2 b, float size)
{
	vec2 dir = a - b;
	vec2 angle = glm::normalize(vec2(dir.y, -dir.x));
	vec2 aa = a + (angle * size);
	vec2 bb = b + (angle * size);

	ImDrawPushQuad(a, b, bb, aa);
}

void ImDrawRectOutline(Rect rect, float size)
{
	// top
	ImDrawLine(rect.pos, rect.pos + vec2(rect.width, 0.0f), size);

	// right
	ImDrawLine(rect.pos + vec2(rect.width, 0.0f), rect.pos + vec2(rect.width, rect.height), size);

	// bottom
	ImDrawLine(rect.pos + vec2(0.0f, rect.height), rect.pos + vec2(rect.width, rect.height), size);

	// left
	ImDrawLine(rect.pos, rect.pos + vec2(0.0f, rect.height), size);
}

void ImDrawRect(Rect rect)
{
    vec2 p1 = vec2(rect.pos.x, rect.pos.y);
    vec2 p2 = vec2(rect.pos.x, rect.pos.y + rect.height);
    vec2 p3 = vec2(rect.pos.x + rect.width, rect.pos.y + rect.height);
    vec2 p4 = vec2(rect.pos.x + rect.width, rect.pos.y);
    ImDrawPushQuad(p1, p2, p3, p4);
}

void ImDrawText(vec2 pos, char *text)
{
	// draw black default text
	ImDrawText(pos, text, vec3(0, 0, 0));
}

void ImDrawText(vec2 pos, char *text, vec3 color)
{
	TextCommand textCommand = {};
	sprintf(textCommand.text, "%s", text);
	textCommand.pos = pos;
	textCommand.color = color;
	imTextCommands.Insert(textCommand);
}

void _ImDrawText(float x, float y, char *text, vec3 color)
{
	// assume orthographic projection with units = screen pixels, origin at top left
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ftex);
	glUseProgram(textShader);

    GLuint colorLoc = glGetUniformLocation(textShader, "textColor");
    glUniform3fv(colorLoc, 1, glm::value_ptr(color));

	while (*text) {
		if (*text >= 32 && *text < 128) {
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(cdata, 512,512, *text-32, &x,&y,&q,1);//1=opengl & d3d10+,0=d3d9

			GLfloat vertices[] = {
				q.x0, q.y0, q.s0, q.t0, 
				q.x1, q.y0, q.s1, q.t0, 
				q.x1, q.y1, q.s1, q.t1, 
				q.x0, q.y1, q.s0, q.t1 
			};

			GLuint indices[] = {
				0, 1, 2,
				2, 3, 0
			};

			glBindVertexArray(imdrawTextVAO);

			glBindBuffer(GL_ARRAY_BUFFER, imdrawTextVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, imdrawTextEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			glBindVertexArray(0);

			glBindVertexArray(imdrawTextVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		++text;
	}

	glUseProgram(0);
	glDisable(GL_BLEND);
}

void ImDrawRender()
{
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_SCISSOR_TEST);

	glUseProgram(imDrawShader);
	for(int i = 0; i < ARRAY_SIZE(imDrawLayers); i++) {

		// if(imDrawVertices.Count() > 0) {
		// 	glUseProgram(imDrawShader);
		// 	glBindVertexArray(imDrawVAO);
		// 	glBindBuffer(GL_ARRAY_BUFFER, imDrawVBO);

		// 	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * imDrawVertices.Count(), imDrawVertices.Data(), GL_DYNAMIC_DRAW);
		// 	glBindVertexArray(0);

		// 	glBindVertexArray(imDrawVAO);
		// 	glDrawArrays(GL_TRIANGLES, 0, imDrawVertices.Count() / 6);
		// 	glBindVertexArray(0);

		// 	imDrawVertices.Clear();
		// 	glUseProgram(0);
		// }

		if(imDrawLayers[i].vertices.Count() > 0) {
			glBindVertexArray(imDrawVAO);
			glBindBuffer(GL_ARRAY_BUFFER, imDrawVBO);

			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * imDrawLayers[i].vertices.Count(), imDrawLayers[i].vertices.Data(), GL_DYNAMIC_DRAW);
			glBindVertexArray(0);

			glBindVertexArray(imDrawVAO);
			glDrawArrays(GL_TRIANGLES, 0, imDrawLayers[i].vertices.Count() / 6);
			glBindVertexArray(0);

			imDrawLayers[i].vertices.Clear();
		}
	}
	glUseProgram(0);

	// glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);
	// glDepthMask(false);

	for(int i = 0; i < imTextCommands.Count(); i++) {
		TextCommand *command = &imTextCommands[i];
		_ImDrawText(command->pos.x, command->pos.y, command->text, command->color);
	}
	imTextCommands.Clear();
}

void imDrawClean()
{
    // imDrawVertices.Free();
}
