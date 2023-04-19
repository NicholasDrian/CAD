#pragma once

#include <vector>
#include <memory>

#include "GL/glew.h"
#include "Shader.h"

enum class ShaderProgramType {
	None,
	BasicTriShader,
	ScreenSpaceTriShader,
	BasicLineShader,
	DashedLineShader,
	BasicPointShader,
	TexturedShader
};

class ShaderProgram {

public:
	ShaderProgram() = default;
	ShaderProgram(ShaderProgramType type);
	void Destroy();

	void Bind();

	inline GLuint GetID() const { return m_ID; }

protected:

	void Init(const char* vertPath, const char* fragPath);

	GLuint m_ID;

};