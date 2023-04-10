#pragma once

#include <vector>
#include <memory>

#include "GL/glew.h"
#include "Shader.h"

enum class ShaderProgramType {
	None,
	ColoredTriShader,
	ColoredLineShader,
	ColoredPointShader,
	TexturedShader
};

class ShaderProgram {

public:
	ShaderProgram() = default;
	ShaderProgram(ShaderProgramType type);
	void Destroy();

	void Bind();

	inline GLuint GetID() const { return m_ID; }
	inline GLuint GetViewProjLocation() const { return m_ViewProjLocation; }

protected:

	void Init(const char* vertPath, const char* fragPath);

	GLuint m_ID;
	GLuint m_ViewProjLocation;

};