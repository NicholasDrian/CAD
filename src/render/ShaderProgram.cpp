#pragma once

#include "ShaderProgram.h"

#include "../utils/OpenGLUtils.h"


ShaderProgram::ShaderProgram(const std::vector<Shader*>& shaders) {
	m_ID = GLResult(glCreateProgram());
	for (Shader* shader : shaders) {
		GLCall(glAttachShader(m_ID, shader->GetID()));
	}
	GLCall(glLinkProgram(m_ID));

#ifdef CAD_DEBUG
	GLint linked;
	GLCall(glGetProgramiv(m_ID, GL_LINK_STATUS, &linked));
	if (linked != GL_TRUE) PrintProgramInfoLog(m_ID);
#endif
}


ShaderProgram::~ShaderProgram()
{
	GLCall(glDeleteProgram(m_ID));
}

GLuint ShaderProgram::GetID() const
{
	return m_ID;
}
