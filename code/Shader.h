#pragma once

struct Shader
{
	GLuint id;

	Shader() {}

	Shader(const char *vPath, const char *fPath)
	{
		// TODO (rhoe) move all shader gl code here
		id = CreateShaderProgram(vPath, fPath);
	}

	// create a compute shader
	Shader(const char *cPath)
	{
		id = glCreateProgram();
		GLuint computeShader = CreateShader(cPath, GL_COMPUTE_SHADER);
		glAttachShader(id, computeShader);
		glLinkProgram(id);
		glDeleteShader(computeShader);
	}

	void Begin()
	{
		glUseProgram(id);
	}

	void End()
	{
		glUseProgram(0);
	}

	GLuint GetUniformLoc(const char *name)
	{
		GLuint loc = glGetUniformLocation(id, name);
		return loc;
	}

	void SetUniform(const char *name, i32 value)
	{
		GLuint loc = GetUniformLoc(name);
		glUniform1i(loc, value);
	}

	void SetUniform(const char *name, float value)
	{
		GLuint loc = GetUniformLoc(name);
		glUniform1f(loc, value);
	}

	void SetUniform(const char *name, mat3 value)
	{
		GLuint loc = GetUniformLoc(name);
		glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	}

	void SetUniform(const char *name, mat4 value)
	{
		GLuint loc = GetUniformLoc(name);
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	}

	void SetUniform(const char *name, vec3 value)
	{
		GLuint loc = GetUniformLoc(name);
		glUniform3fv(loc, 1, glm::value_ptr(value));
	}

	void SetUniform(const char *name, vec4 value)
	{
		GLuint loc = GetUniformLoc(name);
		glUniform4fv(loc, 1, glm::value_ptr(value));
	}

};
