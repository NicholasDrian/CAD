#pragma once

#include "ShaderProgram.h"

#include "GL/glew.h"

#include <unordered_map>

class ShaderManager {

public:

	ShaderManager();
	~ShaderManager();

	void Init();

	void Bind(ShaderProgramType type);

	void UpdateViewProj();

private:

	ShaderProgramType m_BoundProgram;
	std::unordered_map<ShaderProgramType, ShaderProgram> m_Programs;
	GLuint m_GlobalShaderUniforms;

};
