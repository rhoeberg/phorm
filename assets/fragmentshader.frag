#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D outTexture;

void main()
{
	vec3 col = vec3(1, 1, 0);
    /* color = texture(outTexture, TexCoords); */
    color = vec4(col, 1.0f);
}
