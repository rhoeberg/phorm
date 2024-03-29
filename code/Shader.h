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

	Shader(const char *cPath, PArray<String> defines)
	{
		id = glCreateProgram();
		GLuint computeShader = CreateShader(cPath, defines, GL_COMPUTE_SHADER);
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
		/* Begin(); */
		GLuint loc = glGetUniformLocation(id, name);
		return loc;
		/* End(); */
	}

	void SetUniform(const char *name, i32 value)
	{
		/* Begin(); */
		GLuint loc = GetUniformLoc(name);
		glUniform1i(loc, value);
		/* End(); */
	}

	void SetUniform(const char *name, float value)
	{
		/* Begin(); */
		GLuint loc = GetUniformLoc(name);
		glUniform1f(loc, value);
		/* End(); */
	}

	void SetUniform(const char *name, mat3 value)
	{
		/* Begin(); */
		GLuint loc = GetUniformLoc(name);
		glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
		/* End(); */
	}

	void SetUniform(const char *name, mat4 value)
	{
		/* Begin(); */
		GLuint loc = GetUniformLoc(name);
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
		/* End(); */
	}

	void SetUniform(const char *name, vec2 value)
	{
		/* Begin(); */
		GLuint loc = GetUniformLoc(name);
		glUniform2fv(loc, 1, glm::value_ptr(value));
		/* End(); */
	}

	void SetUniform(const char *name, vec3 value)
	{
		/* Begin(); */
		GLuint loc = GetUniformLoc(name);
		glUniform3fv(loc, 1, glm::value_ptr(value));
		/* End(); */
	}

	void SetUniform(const char *name, vec4 value)
	{
		/* Begin(); */
		GLuint loc = GetUniformLoc(name);
		glUniform4fv(loc, 1, glm::value_ptr(value));
		/* End(); */
	}

	void SetUniform(const char *name, i32 a, i32 b)
	{
		/* Begin(); */
		GLuint loc = GetUniformLoc(name);
		glUniform2i(loc, a, b);
		/* End(); */
	}

};
