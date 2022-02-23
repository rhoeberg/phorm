#version 330 core

in vec2 TexCoord;
out vec4 color;

uniform sampler2D outTexture;

void main()
{
	vec4 result = texture(outTexture, TexCoord);
	color = result;
}
