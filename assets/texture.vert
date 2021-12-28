#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

void main()
{
	TexCoord = uv;
	FragPos = vec3(model * vec4(position, 1.0));
	Normal = normal * normalMatrix;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
