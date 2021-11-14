// vec2 *ImDrawVertices = NULL;
// GLfloat *ImDrawVertices = NULL;
#include <vector>
std::vector<GLfloat> ImDrawVertices;
GLuint ImDrawVAO;
GLuint ImDrawVBO;
GLuint ImDrawShader;

void ImDrawInitialize(glm::mat4 view, glm::mat4 projection)
{
    ImDrawShader = createShaderProgram("assets\\imdraw.vs", "assets\\imdraw.frag");
    
    glGenVertexArrays(1, &ImDrawVAO);
    glGenBuffers(1, &ImDrawVBO);

    glUseProgram(ImDrawShader);
    GLuint viewLoc = glGetUniformLocation(ImDrawShader, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    GLuint projectionLoc = glGetUniformLocation(ImDrawShader, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    GLuint modelLoc = glGetUniformLocation(ImDrawShader, "model");
    // glm::mat4 model;
	glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
    model = glm::translate(model, glm::vec3(0,0,0));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void ImDrawSetColor(vec3 color)
{
	GLuint colorLoc = glGetUniformLocation(ImDrawShader, "color");
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));
}

void ImDrawPushQuad(vec2 p1, vec2 p2, vec2 p3, vec2 p4)
{
    // triangle 1
    ImDrawVertices.push_back(p1.x);
    ImDrawVertices.push_back(p1.y);
    ImDrawVertices.push_back(p2.x);
    ImDrawVertices.push_back(p2.y);
    ImDrawVertices.push_back(p3.x);
    ImDrawVertices.push_back(p3.y);
    // triangle 2
    ImDrawVertices.push_back(p3.x);
    ImDrawVertices.push_back(p3.y);
    ImDrawVertices.push_back(p4.x);
    ImDrawVertices.push_back(p4.y);
    ImDrawVertices.push_back(p1.x);
    ImDrawVertices.push_back(p1.y);
}

void ImDrawPushTriangle(vec2 p1, vec2 p2, vec2 p3)
{
    ImDrawVertices.push_back(p1.x);
    ImDrawVertices.push_back(p1.y);
    ImDrawVertices.push_back(p2.x);
    ImDrawVertices.push_back(p2.y);
    ImDrawVertices.push_back(p3.x);
    ImDrawVertices.push_back(p3.y);
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

void ImDrawLine(vec2 a, vec2 b,float size = 3.0f)
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
    if(ImDrawVertices.size() > 0) {

		glUseProgram(ImDrawShader);
		glBindVertexArray(ImDrawVAO);
		glBindBuffer(GL_ARRAY_BUFFER, ImDrawVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * ImDrawVertices.size(), ImDrawVertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		glBindVertexArray(ImDrawVAO);
		glDrawArrays(GL_TRIANGLES, 0, ImDrawVertices.size() / 2);
		glBindVertexArray(0);

		ImDrawVertices.clear();
    }
	glEnable(GL_DEPTH_TEST);
}

void imDrawClean()
{
    ImDrawVertices.clear();
}
