#version 330 core

in vec2 TexCoords;
out vec4 Color;

void main()
{
	vec3 col = vec3(1, 1, 0);
    Color = vec4(col, 1);
}
