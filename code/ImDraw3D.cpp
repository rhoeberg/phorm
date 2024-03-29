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
	new(&_im3D->vertices) PArray<GLfloat>();

	// SETUP SHADER
    _im3D->shader = CreateShaderProgram("assets/shaders/imdraw3d.vs", "assets/shaders/imdraw3d.frag");

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


	// set default color
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

void ImDraw3DPlane(vec3 pos, vec3 normal, float size)
{
	vec3 forward = vec3(0, 0, 1);
	if(normal != WORLD_UP) {
		forward = glm::cross(WORLD_UP, normal);
	}
	vec3 right = glm::cross(normal, forward);

	vec3 a = pos - (right * size) - (forward * size);
	vec3 b = pos - (right * size) + (forward * size);
	vec3 c = pos + (right * size) + (forward * size);
	vec3 d = pos + (right * size) - (forward * size);

	ImDraw3DPushQuad(a, b, c, d);
}


void ImDraw3DCone(vec3 start, vec3 end, float thickness, int resolution)
{
    float deltaAngle = 2.0 * PI / resolution;
	// mat3 rotation = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	vec3 dir = glm::normalize(end - start);
	// if(dir != WORLD_UP && dir != vec3(0, -1, 0)) {
		// rotation = glm::lookAt(start, end, WORLD_UP);
	// }
	mat3 rotation = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	if(dir != WORLD_UP && dir != vec3(0, -1, 0)) {
		vec3 axis = glm::cross(WORLD_UP, dir);
		float angle = glm::angle(WORLD_UP, dir);
		rotation = glm::rotate(angle, axis);
	}

	{
		// draw bottom
		vec3 point0;
		point0.x = cos(deltaAngle * 0) * thickness;
		point0.z = sin(deltaAngle * 0) * thickness;
		point0.y = 0.0f;
		point0 = rotation * point0;
		for(i32 i = 2; i < resolution; i++) {
			// point 1
			vec3 point1 = vec3(0, 0, 0);
			point1.x = cos(deltaAngle * (i - 1)) * thickness;
			point1.z = sin(deltaAngle * (i - 1)) * thickness;
			point1 = rotation * point1;

			// point 2
			vec3 point2 = vec3(0, 0, 0);
			point2.x = cos(deltaAngle * i) * thickness;
			point2.z = sin(deltaAngle * i) * thickness;
			point2 = rotation * point2;

			ImDraw3DPushTriangle(point0 + start, point1 + start, point2 + start);
		}
	}

	// draw sides
	for(i32 i = 0; i < resolution; i++) {
		// point 1
		vec3 point0 = vec3(0, 0, 0);
		point0.x = cos(deltaAngle * (i - 1)) * thickness;
		point0.z = sin(deltaAngle * (i - 1)) * thickness;
		point0 = rotation * point0;

		// point 2
		vec3 point1 = vec3(0, 0, 0);
		point1.x = cos(deltaAngle * i) * thickness;
		point1.z = sin(deltaAngle * i) * thickness;
		point1 = rotation * point1;

		ImDraw3DPushTriangle(point0 + start, point1 + start, end);

		if(i == resolution) {
			// draw from end to beginning
		}
	}
}

void ImDraw3DCylinder(vec3 start, vec3 end, float thickness, int resolution)
{
	// we make a rotation matrix from a normal pointing in "world up"
	// to normal pointing in the given direction
	vec3 dir = glm::normalize(end - start);
	mat3 rotation = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	if(dir != WORLD_UP && dir != vec3(0, -1, 0)) {
		vec3 axis = glm::cross(WORLD_UP, dir);
		float angle = glm::angle(WORLD_UP, dir);
		rotation = glm::rotate(angle, axis);
	}

    float deltaAngle = 2.0 * PI / resolution;

    vec3 firstBottom;
    vec3 firstTop;
    vec3 oldBottom;
    vec3 oldTop;
    for(i32 i = 0; i < resolution; i++) {
        vec3 newP;
        newP.x = cos(deltaAngle * i) * thickness;
        newP.z = sin(deltaAngle * i) * thickness;
        newP.y = 0.0f;
        
        newP = rotation * newP;
        
        vec3 bottom = newP + start;
        vec3 top = newP + end;

        
        if(i == 0) {
            firstBottom = bottom;
            firstTop = top;
        }
        else {
            vec3 p1 = oldBottom;
            vec3 p2 = bottom;
            vec3 p3 = top;
            vec3 p4 = oldTop;
            ImDraw3DPushQuad(p1, p2, p3, p4);
        }
        oldBottom = bottom;
        oldTop = top;
    }
    
    vec3 p1 = oldBottom;
    vec3 p2 = firstBottom;
    vec3 p3 = firstTop;
    vec3 p4 = oldTop;
    ImDraw3DPushQuad(p1, p2, p3, p4);

	// draw top and bottom
	vec3 point0;
	point0.x = cos(deltaAngle * 0) * thickness;
	point0.z = sin(deltaAngle * 0) * thickness;
	point0.y = 0.0f;
	point0 = rotation * point0;
	for(i32 i = 2; i < resolution; i++) {
		// point 1
		vec3 point1 = vec3(0, 0, 0);
		point1.x = cos(deltaAngle * (i - 1)) * thickness;
		point1.z = sin(deltaAngle * (i - 1)) * thickness;
		point1 = rotation * point1;

		// point 2
		vec3 point2 = vec3(0, 0, 0);
		point2.x = cos(deltaAngle * i) * thickness;
		point2.z = sin(deltaAngle * i) * thickness;
		point2 = rotation * point2;

		ImDraw3DPushTriangle(point0 + start, point1 + start, point2 + start);
		ImDraw3DPushTriangle(point2 + end, point1 + end, point0 + end);
	}
}

void ImDraw3DArrow(vec3 start, vec3 end, float thickness = 0.01f)
{
	int resolution = 10;
	// float thickness = 0.03f;

	vec3 dir = glm::normalize(end - start);
	float length = glm::distance(start, end);

	float coneHeight = 0.05f;
	if(length < 0.05f)
		coneHeight = length / 2.0f;

	float lengthCyl = length - coneHeight;

	ImDraw3DCylinder(start, start + (dir * lengthCyl), thickness, resolution);
	ImDraw3DCone(start + (dir * lengthCyl), end, thickness * 3, resolution);
}


/* 
   Draw a axis aligned cube with evenly sized faces
*/
void ImDraw3DCube(vec3 pos, float size)
{
	// FRONT
	vec3 frontOffset = pos - vec3(0, 0, size);
	vec3 frontBottomLeft = frontOffset + vec3(-size, -size, 0);
	vec3 frontBottomRight = frontOffset + vec3(size, -size, 0);
	vec3 frontTopRight = frontOffset + vec3(size, size, 0);
	vec3 frontTopLeft = frontOffset + vec3(-size, size, 0);
	ImDraw3DPushQuad(frontBottomLeft, frontBottomRight, frontTopRight, frontTopLeft);

	// BACK
	vec3 backBottomRight = pos + vec3(size, -size, size);
	vec3 backBottomLeft = pos + vec3(-size, -size, size);
	vec3 backTopLeft = pos + vec3(-size, size, size);
	vec3 backTopRight = pos + vec3(size, size, size);
	ImDraw3DPushQuad(backBottomLeft, backBottomRight, backTopRight, backTopLeft);

	// LEFT
	vec3 leftBottomBack = pos + vec3(-size, -size, -size);
	vec3 leftBottomFront = pos + vec3(-size, -size, size);
	vec3 leftTopFront = pos + vec3(-size, size, size);
	vec3 leftTopBack = pos + vec3(-size, size, -size);
	ImDraw3DPushQuad(leftBottomBack, leftBottomFront, leftTopFront, leftTopBack);

	// RIGHT
	vec3 rightBottomFront = pos + vec3(size, -size, size);
	vec3 rightBottomBack = pos + vec3(size, -size, -size);
	vec3 rightTopBack = pos + vec3(size, size, -size);
	vec3 rightTopFront = pos + vec3(size, size, size);
	ImDraw3DPushQuad(rightBottomFront, rightBottomBack, rightTopBack, rightTopFront);

	// TOP
	vec3 topFrontLeft = pos + vec3(-size, size, -size);
	vec3 topFrontRight = pos + vec3(size, size, -size);
	vec3 topBackRight = pos + vec3(size, size, size);
	vec3 topBackLeft = pos + vec3(-size, size, size);
	ImDraw3DPushQuad(topFrontLeft, topFrontRight, topBackRight, topBackLeft);

	// BOTTOM
	vec3 bottomFrontRight = pos + vec3(size, -size, -size);
	vec3 bottomFrontLeft = pos + vec3(-size, -size, -size);
	vec3 bottomBackLeft = pos + vec3(-size, -size, size);
	vec3 bottomBackRight = pos + vec3(size, -size, size);
	ImDraw3DPushQuad(bottomFrontRight, bottomFrontLeft, bottomBackLeft, bottomBackRight);
}

void ImDraw3DRender()
{
    glDisable(GL_DEPTH_TEST);

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

    glEnable(GL_DEPTH_TEST);
}

void ImDraw3DCleanup()
{
	_im3D->vertices.Free();
	free(_im3D);
}
