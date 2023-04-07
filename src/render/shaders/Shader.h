#pragma once

#include "GL/glew.h"

enum class ShaderStage {
	VertexShader,
	FragmentShader
};

class Shader {

public:

	Shader(const char* path, ShaderStage type);
	~Shader();

	GLuint GetID() const;

	static GLenum GetGLShaderType(ShaderStage type);

private:

	GLuint m_ID;

};