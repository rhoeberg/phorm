/*
  imdraw.cpp should be should be changed into a 2d drawing libary
  (thats basically what it is anyways)
  this version of imdraw can keep its own projection and view matrices

  we need a orthographic projection to get the correct coordinate system
  this should give is the coords:

(0, 0)
  -----------------
  |               |                
  |               |
  |               |
  |               |
  |               |
  -----------------
              (WIDTH, HEIGHT)

			  

  Later we will write a "in world" version which takes projection and view 
  from the scene renderer
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

// vec2 *imDrawVertices = NULL;
// GLfloat *imDrawVertices = NULL;
#include <vector>

#define MAX_TEXT_SIZE 512

struct TextCommand {
	char text[MAX_TEXT_SIZE];
	vec2 pos;
};

global std::vector<GLfloat> imDrawVertices; // TODO (rhoe) remove std dependencies
global GLuint imDrawVAO;
global GLuint imDrawVBO;
global GLuint imDrawShader;
global vec3 imDrawColor;
global std::vector<TextCommand> imTextCommands;

void ImDrawSetWindowSize(int width, int height)
{
    glm::mat4 projection;
    projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
	glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
    model = glm::translate(model, glm::vec3(0,0,0));

    glUseProgram(imDrawShader);
    GLuint projectionLoc = glGetUniformLocation(imDrawShader, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    GLuint modelLoc = glGetUniformLocation(imDrawShader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

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

    // GLuint viewLoc = glGetUniformLocation(imDrawShader, "view");
    // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	int width, height;
	GetWindowSize(&width, &height);
	ImDrawSetWindowSize(width, height);

	// INITIALIZE FONT DRAWING
	FILE *file = fopen("c:/windows/fonts/consola.ttf", "rb");
	if(!file) {
		printf("font file not found\n");
	}
	fread(ttf_buffer, 1, 1<<20, file);
	stbtt_BakeFontBitmap(ttf_buffer,0, 20.0, temp_bitmap,512,512, 32,96, cdata); // no guarantee this fits!
	// can free ttf_buffer at this point

	glGenTextures(1, &ftex);
	glBindTexture(GL_TEXTURE_2D, ftex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512,512, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
	// can free temp_bitmap at this point
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void ImDrawSetColor(vec3 color)
{
	// GLuint colorLoc = glGetUniformLocation(ImDrawShader, "color");
	// glUniform3fv(colorLoc, 1, glm::value_ptr(color));
	imDrawColor = color;
}

void ImDrawAddVertex(vec2 v)
{
	imDrawVertices.push_back(v.x);
	imDrawVertices.push_back(v.y);
	imDrawVertices.push_back(imDrawColor.r);
	imDrawVertices.push_back(imDrawColor.g);
	imDrawVertices.push_back(imDrawColor.b);
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


void ImDrawLine(vec2 a, vec2 b,float size = 1.0f)
{
	vec2 dir = a - b;
	vec2 angle = glm::normalize(vec2(dir.y, -dir.x));
	vec2 aa = a + (angle * size);
	vec2 bb = b + (angle * size);

	ImDrawPushQuad(a, b, bb, aa);
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
	TextCommand textCommand = {};
	sprintf(textCommand.text, "%s", text);
	textCommand.pos = pos;
	imTextCommands.push_back(textCommand);
}

void _ImDrawText(float x, float y, char *text)
{
	// assume orthographic projection with units = screen pixels, origin at top left
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ftex);
	glUseProgram(textShader);

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

			GLuint VAO, VBO, EBO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glBindVertexArray(0);

			glBindVertexArray(VAO);
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
	glDisable(GL_DEPTH_TEST);
    if(imDrawVertices.size() > 0) {
		glUseProgram(imDrawShader);
		glBindVertexArray(imDrawVAO);
		glBindBuffer(GL_ARRAY_BUFFER, imDrawVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * imDrawVertices.size(), imDrawVertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);

		glBindVertexArray(imDrawVAO);
		glDrawArrays(GL_TRIANGLES, 0, imDrawVertices.size() / 2);
		glBindVertexArray(0);

		imDrawVertices.clear();
		glUseProgram(0);
    }
	glEnable(GL_DEPTH_TEST);

	for(int i = 0; i < imTextCommands.size(); i++) {
		TextCommand *command = &imTextCommands[i];
		_ImDrawText(command->pos.x, command->pos.y, command->text);
	}
	imTextCommands.clear();
}

void imDrawClean()
{
    imDrawVertices.clear();
}
