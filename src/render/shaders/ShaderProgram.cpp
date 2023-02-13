#pragma once

#include "ShaderProgram.h"
#include <stdexcept>
#include "../OpenGLUtils.h"


ShaderProgram::ShaderProgram()
{
	//throw std::runtime_error("Should not be creating a shader program");
}

void ShaderProgram::Destroy()
{
	GLCall(glDeleteProgram(m_ID));
}

void ShaderProgram::Bind()
{
	GLCall(glUseProgram(m_ID));
}

void ShaderProgram::Init(const char* vertPath, const char* fragPath) 
{
	Shader vertShader(vertPath, ShaderType::VertexShader);
	Shader fragShader(fragPath, ShaderType::FragmentShader);

	m_ID = GLResult(glCreateProgram());
	GLCall(glAttachShader(m_ID, vertShader.GetID()));
	GLCall(glAttachShader(m_ID, fragShader.GetID()));
	GLCall(glLinkProgram(m_ID));

#ifdef CAD_DEBUG
	GLint linked;
	GLCall(glGetProgramiv(m_ID, GL_LINK_STATUS, &linked));
	if (linked != GL_TRUE) PrintProgramInfoLog(m_ID);
#endif

	m_ViewProjLocation = GLResult(glGetUniformLocation(m_ID, "view_proj"));
}