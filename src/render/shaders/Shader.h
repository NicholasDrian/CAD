#pragma once

#include "GL/glew.h"

enum class ShaderType {
	VertexShader,
	FragmentShader
};

class Shader {

public:

	Shader(const char* path, ShaderType type);
	~Shader();

	GLuint GetID() const;

	static GLenum GetGLShaderType(ShaderType type);

private:

	GLuint m_ID;

};