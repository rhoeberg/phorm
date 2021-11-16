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

// vec2 *imDrawVertices = NULL;
// GLfloat *imDrawVertices = NULL;
#include <vector>
std::vector<GLfloat> imDrawVertices; // TODO (rhoe) remove std dependencies
global GLuint imDrawVAO;
global GLuint imDrawVBO;
global GLuint imDrawShader;

global vec3 ImDrawColor;

void ImDrawInitialize()
{
    imDrawShader = createShaderProgram("assets\\imdraw.vs", "assets\\imdraw.frag");
    
    glGenVertexArrays(1, &imDrawVAO);
    glGenBuffers(1, &imDrawVBO);

    glUseProgram(imDrawShader);
    // GLuint viewLoc = glGetUniformLocation(imDrawShader, "view");
    // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    GLuint projectionLoc = glGetUniformLocation(imDrawShader, "projection");
    glm::mat4 projection;
    projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT, -1.0f, 1.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    GLuint modelLoc = glGetUniformLocation(imDrawShader, "model");
    // glm::mat4 model;
	glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
    model = glm::translate(model, glm::vec3(0,0,0));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void ImDrawSetColor(vec3 color)
{
	// GLuint colorLoc = glGetUniformLocation(ImDrawShader, "color");
	// glUniform3fv(colorLoc, 1, glm::value_ptr(color));
	ImDrawColor = color;
}

void ImDrawAddVertex(vec2 v)
{
	imDrawVertices.push_back(v.x);
	imDrawVertices.push_back(v.y);
	imDrawVertices.push_back(ImDrawColor.r);
	imDrawVertices.push_back(ImDrawColor.g);
	imDrawVertices.push_back(ImDrawColor.b);
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


void ImDrawLine(vec2 a, vec2 b,float size = 0.01f)
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
    }
	glEnable(GL_DEPTH_TEST);
}

void imDrawClean()
{
    imDrawVertices.clear();
}
