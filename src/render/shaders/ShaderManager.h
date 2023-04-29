#pragma once

#include "ShaderProgram.h"
#include "glm/matrix.hpp"
#include "GL/glew.h"

#include <unordered_map>

class ShaderManager {

public:

	static void Init();
	static void Destroy();

	static void Bind(ShaderProgramType type);

	static void UpdateGlobalUniforms();
	static void UpdateLocalUniforms(const glm::mat4& model, const glm::vec4& color, bool selectable, bool subSelectable, bool selected, uint32_t id);

private:

	inline static ShaderProgramType m_BoundProgram = ShaderProgramType::None;
	inline static std::unordered_map<ShaderProgramType, ShaderProgram> m_Programs;
	inline static GLuint m_GlobalShaderUniforms, m_LocalShaderUniforms;

};
