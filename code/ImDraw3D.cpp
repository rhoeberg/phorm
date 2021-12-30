
struct ImDraw3DState
{
	VMArray<GLfloat> vertices;
	vec4 drawColor;
	GLuint shader;
	GLuint vbo;
	int vaoHandle;
};

global ImDraw3DState *_im3D;

void ImDraw3DSetView(mat4 view)
{
	glUseProgram(_im3D->shader);
	GLuint loc = glGetUniformLocation(_im3D->shader, "view");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));
}

void ImDraw3DSetProjection(mat4 projection)
{
	glUseProgram(_im3D->shader);
	GLuint loc = glGetUniformLocation(_im3D->shader, "projection");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));
}

void ImDraw3DInitialize()
{
	_im3D= (ImDraw3DState*)malloc(sizeof(ImDraw3DState));
	new(&_im3D->vertices) VMArray<GLfloat>();

	
	// SETUP SHADER
    _im3D->shader = createShaderProgram("assets\\imdraw3d.vs", "assets\\imdraw3d.frag");
	// GLuint viewLoc = glGetUniformLocation(_im3D->shader, "view");
	// GLuint projectionLoc = glGetUniformLocation(_im3D->shader, "projection");
	// glUseProgram(_im3D->shader);
	

	// SETUP GL BUFFERS
	_im3D->vaoHandle = AddVAO();
	glGenBuffers(1, &_im3D->vbo);


	SetContextMain();
	BindMainContextVAO(_im3D->vaoHandle);
	glBindBuffer(GL_ARRAY_BUFFER, _im3D->vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	SetContextViewer();
	BindViewerContextVAO(_im3D->vaoHandle);
	glBindBuffer(GL_ARRAY_BUFFER, _im3D->vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);



	//
	ImDraw3DSetColor(vec3(1.0f, 1.0f, 1.0f));
}

void ImDraw3DSetColor(vec4 color)
{
	_im3D->drawColor = color;
}

void ImDraw3DSetColor(vec3 color)
{
	ImDraw3DSetColor(vec4(color.x, color.y, color.z, 1.0f));
}

void ImDraw3DAddVertex(vec3 pos)
{
	_im3D->vertices.Insert(pos.x);
	_im3D->vertices.Insert(pos.y);
	_im3D->vertices.Insert(pos.z);
	_im3D->vertices.Insert(_im3D->drawColor.r);
	_im3D->vertices.Insert(_im3D->drawColor.g);
	_im3D->vertices.Insert(_im3D->drawColor.b);
	_im3D->vertices.Insert(_im3D->drawColor.a);
}

void ImDraw3DPushTriangle(vec3 a, vec3 b, vec3 c)
{
	ImDraw3DAddVertex(a);
	ImDraw3DAddVertex(b);
	ImDraw3DAddVertex(c);
}

// remember to do anti clockwise order of vertices
void ImDraw3DPushQuad(vec3 a, vec3 b, vec3 c, vec3 d)
{
	ImDraw3DAddVertex(a);
	ImDraw3DAddVertex(b);
	ImDraw3DAddVertex(c);

	ImDraw3DAddVertex(c);
	ImDraw3DAddVertex(d);
	ImDraw3DAddVertex(a);
}

void ImDraw3DCube(vec3 pos, float size)
{
	// front
	vec3 frontOffset = pos - vec3(0, 0, size);
	vec3 frontBottomLeft = frontOffset + vec3(-size, -size, 0);
	vec3 frontBottomRight = frontOffset + vec3(size, -size, 0);
	vec3 frontTopRight = frontOffset + vec3(size, size, 0);
	vec3 frontTopLeft = frontOffset + vec3(-size, size, 0);
	ImDraw3DPushQuad(frontBottomLeft, frontBottomRight, frontTopRight, frontTopLeft);

	// back
	vec3 backOffset = pos + vec3(0, 0, size);
	vec3 backBottomRight = backOffset + vec3(size, -size, 0);
	vec3 backBottomLeft = backOffset + vec3(-size, -size, 0);
	vec3 backTopLeft = backOffset + vec3(-size, size, 0);
	vec3 backTopRight = backOffset + vec3(size, size, 0);
	ImDraw3DPushQuad(frontBottomLeft, frontBottomRight, frontTopRight, frontTopLeft);

	// left
	vec3 leftBottomBack = pos + vec3(-size, -size, -size);
	vec3 leftBottomFront = pos + vec3(-size, -size, size);
	vec3 leftTopFront = pos + vec3(-size, size, size);
	vec3 leftTopBack = pos + vec3(-size, size, -size);
	ImDraw3DPushQuad(leftBottomBack, leftBottomFront, leftTopFront, leftTopBack);

	// rigth
	vec3 rightBottomFront = pos + vec3(size, -size, size);
	vec3 rightBottomBack = pos + vec3(size, -size, -size);
	vec3 rightTopBack = pos + vec3(size, size, -size);
	vec3 rightTopFront = pos + vec3(size, size, size);
	ImDraw3DPushQuad(rightBottomFront, rightBottomBack, rightTopBack, rightTopFront);
}

void ImDraw3DRender()
{
	glUseProgram(_im3D->shader);

	glBindVertexArray(GetCurrentContextVAO(_im3D->vaoHandle));
	glBindBuffer(GL_ARRAY_BUFFER, _im3D->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _im3D->vertices.Count(), _im3D->vertices.Data(), GL_DYNAMIC_DRAW);
	glBindVertexArray(0);


	GLuint loc = glGetUniformLocation(_im3D->shader, "model");
	glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));
	
	glBindVertexArray(GetCurrentContextVAO(_im3D->vaoHandle));
	glDrawArrays(GL_TRIANGLES, 0, _im3D->vertices.Count() / 7);
	glBindVertexArray(0);

	_im3D->vertices.Clear();
}

void ImDraw3DCleanup()
{
	_im3D->vertices.Free();
	free(_im3D);
}
