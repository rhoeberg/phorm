#version 330 core

in vec3 COLOR;
out vec4 Color;

/* uniform vec3 color; */

void main()
{
    /* Color = vec4(1.0f, 1.0f, 0.0f, 1.0f); */
    /* Color = vec4(color, 0.0f); */
    Color = vec4(COLOR, 1.0f);
}
