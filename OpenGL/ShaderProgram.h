#pragma once

#include<GL/glew.h>
#include<glm/glm.hpp> // OpenGL Mathematics Library.
#include<glm/gtc/type_ptr.hpp>

#include<string>
#include<assert.h>
#include<fstream> // File input.
#include<sstream> // From file to string.
#include<map>

#include"ColorConsole.h"


class ShaderProgram {
public:
	enum class ShaderType {
		SHADER_TYPE_VERTEXSHADER,
		SHADER_TYPE_FRAGMENTSHADER,
		SHADER_TYPE_SHADERPROGRAM
	};


public:

	~ShaderProgram();

	bool LoadShaders(const char* vsFilename, const char* fsFilename);

	void Use();


	// For shader static variables.
	void SetUniform(const GLchar* name, const glm::vec2& v);
	void SetUniform(const GLchar* name, const glm::vec3& v);
	void SetUniform(const GLchar* name, const glm::vec4& v);

	void SetUniform(const GLchar* name, const glm::mat4& m); // For matrices.

	void SetUniform(const GLchar* name, const GLfloat& f);
	void SetUniform(const GLchar* name, const GLint& i);

	void SetUniformSampler(const GLchar* name, const GLint& textureSlot);


	GLuint GetProgram()const { return m_ProgramHandle; }

private:

	// Map for storing uniforms.
	// Locations shall not change during program.
	std::map<std::string, GLint> m_UniformLocationsMap;

	GLuint m_ProgramHandle = NULL;

private:

	std::string _fileToString(const std::string& filename);
	void _compilingCheck(GLuint shaderObj, ShaderType sType);

	// Utility for uniform variables.
	GLint _getUniformLocation(const GLchar* name);
};
