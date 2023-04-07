#pragma once

#include "ShaderProgram.h"
#include <stdexcept>
#include "../OpenGLUtils.h"


ShaderProgram::ShaderProgram(ShaderProgramType type)
{
	const char* vertPath;
	const char* fragPath;

	switch (type) {
	case ShaderProgramType::ColoredTriShader:
		vertPath = "src/render/shaders/shaders/ColoredTri.vert";
		fragPath = "src/render/shaders/shaders/ColoredTri.frag";
		break;
	case ShaderProgramType::ColoredLineShader:
		vertPath = "src/render/shaders/shaders/ColoredLine.vert";
		fragPath = "src/render/shaders/shaders/ColoredLine.frag";
		break;
#ifdef CAD_DEBUG
	default:
		throw std::runtime_error("Your fired!");
#endif
	}

	Shader vertShader(vertPath, ShaderStage::VertexShader);
	Shader fragShader(fragPath, ShaderStage::FragmentShader);

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
	Shader vertShader(vertPath, ShaderStage::VertexShader);
	Shader fragShader(fragPath, ShaderStage::FragmentShader);

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