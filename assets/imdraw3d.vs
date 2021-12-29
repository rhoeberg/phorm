#version  330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 COLOR;

void main()
{
    gl_Position = projection * model * view * vec4(position, 1.0f);
	COLOR = color;
}
