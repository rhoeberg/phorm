#version 330 core

in vec2 TexCoord;
out vec4 color;

uniform sampler2D outTexture;
uniform vec4 objectColor;

void main()
{
	vec4 result = texture(outTexture, TexCoord);
	result *= objectColor;
	color = result;
}
