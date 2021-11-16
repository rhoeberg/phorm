#version  330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

/* uniform mat4 model; */
uniform mat4 view;
uniform mat4 projection;

out vec3 COLOR;

void main()
{
    /* gl_Position = projection * model * vec4(position, 0.0f, 1.0f); */
    gl_Position = vec4(position, 0.0f, 1.0f);
	COLOR = color;
}
