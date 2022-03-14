#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D outTexture;

uniform float WaveAmount;
uniform float WaveFreq;

void main()
{
	/* float amount = 0.1; */

	float freq = 30;
	vec2 waveCoords = TexCoords;
	waveCoords.x = waveCoords.x + (WaveAmount * sin(waveCoords.y * WaveFreq));
    color = texture(outTexture, waveCoords);

    /* color = texture(outTexture, TexCoords); */
}
