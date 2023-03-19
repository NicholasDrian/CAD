#pragma once

#include "ShaderProgram.h"

#include "GL/glew.h"

#include <unordered_map>

class ShaderManager {

public:

	static void Init();
	static void Destroy();

	static void Bind(ShaderProgramType type);

	static void UpdateViewProj();

private:

	inline static ShaderProgramType m_BoundProgram = ShaderProgramType::None;
	inline static std::unordered_map<ShaderProgramType, ShaderProgram> m_Programs;
	inline static GLuint m_GlobalShaderUniforms;

};
