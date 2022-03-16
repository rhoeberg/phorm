#version 330 core

in vec2 TexCoord;
in vec3 FragPos;

in vec3 Normal;
out vec4 color;

#define LIGHT_AMOUNT 32

struct PointLight {
	vec3 pos;
	vec3 color;
};

uniform PointLight pointLights[LIGHT_AMOUNT];
uniform int pointLightAmount;

uniform sampler2D outTexture;
uniform vec4 objectColor;
/* uniform vec3 ambientColor; */

vec3 CalcPointLight(PointLight light, vec3 fragPos, vec3 normal)
{
	vec3 result = vec3(0);
	vec3 lightDir = normalize(light.pos - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * light.color;
	result += diffuse;

	return result;
}

void main()
{
	vec3 normal = normalize(Normal);
	if(!gl_FrontFacing) {
		normal *= -1.0;
	}

	////////////
	// AMBIENT
	vec3 ambientColor = vec3(1, 1, 1);
	float ambientStrength = 0.6;
	vec3 ambient = ambientStrength * ambientColor;

	vec3 result = vec3(0);
	for(int i = 0; i < pointLightAmount; i++) {
		result += CalcPointLight(pointLights[i], FragPos, normal);
	}

	result += ambient;
	result *= texture(outTexture, TexCoord).xyz;
	/* result *= objectColor; */
	color = vec4(result, 1.0) * objectColor;
}
