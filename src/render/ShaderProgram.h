#pragma once

#include <vector>

#include "GL/glew.h"

#include "Shader.h"

class ShaderProgram {

public:

	ShaderProgram(const std::vector<Shader*>& shaders);
	~ShaderProgram();

	GLuint GetID() const;

private:

	GLuint m_ID;

};