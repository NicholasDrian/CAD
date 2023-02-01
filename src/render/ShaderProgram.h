#pragma once

#include <vector>
#include <memory>

#include "GL/glew.h"

#include "Shader.h"

class ShaderProgram {

public:

	ShaderProgram(const Shader& vertShader, const Shader& fragShader);
	~ShaderProgram();

	GLuint GetID() const;

private:

	GLuint m_ID;

};