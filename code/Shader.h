#pragma once

struct Shader
{
	GLuint id;

	Shader() {}

	Shader(const char *vPath, const char *fPath)
	{
		// TODO (rhoe) move all shader gl code here
		id = createShaderProgram(vPath, fPath);
	}

	void Use()
	{
		glUseProgram(id);
	}

	GLuint GetUniformLoc(const char *name)
	{
		GLuint loc = glGetUniformLocation(id, name);
		return loc;
	}

	void SetUniform1i(const char *name, i32 value)
	{
		GLuint loc = GetUniformLoc(name);
		glUniform1i(loc, value);
	}

	void SetUniformMatrix3fv(const char *name, mat3 value)
	{
		GLuint loc = GetUniformLoc(name);
		glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	}

	void SetUniformMatrix4fv(const char *name, mat4 value)
	{
		GLuint loc = GetUniformLoc(name);
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	}

	void SetUniform3fv(const char *name, vec3 value)
	{
		GLuint loc = GetUniformLoc(name);
		glUniform3fv(loc, 1, glm::value_ptr(value));
	}
};
