#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

out vec2 TexCoords;

/* uniform mat4 model; */
/* uniform mat4 view; */
/* uniform mat4 projection; */

void main()
{
    /* gl_Position = projection * model * vec4(position, 1.0f); */
	vec3 newPos = vec3(position.x, -1 * position.y, position.z);
    gl_Position = vec4(newPos, 1.0f);
    /* gl_Position = vec4(position, 1.0f); */
	TexCoords = uv;
}
