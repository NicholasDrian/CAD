#pragma once

#include "ShaderProgram.h"
#include <stdexcept>
#include "../OpenGLUtils.h"

const std::string SHADER_BASE_PATH("src/render/shaders/shaders/");

ShaderProgram::ShaderProgram(ShaderProgramType type)
{
	std::string vertPath;
	std::string fragPath;

	switch (type) {
	case ShaderProgramType::ColoredTriShader:
		vertPath = SHADER_BASE_PATH + "ColoredTri.vert";
		fragPath = SHADER_BASE_PATH + "ColoredTri.frag";
		break;
	case ShaderProgramType::ColoredLineShader:
		vertPath = SHADER_BASE_PATH + "ColoredLine.vert";
		fragPath = SHADER_BASE_PATH + "ColoredLine.frag";
		break;
	case ShaderProgramType::ColoredPointShader:
		vertPath = SHADER_BASE_PATH + "ColoredPoint.vert";
		fragPath = SHADER_BASE_PATH + "ColoredPoint.frag";
		break;
#ifdef CAD_DEBUG
	default:
		throw std::runtime_error("Your fired!");
#endif
	}

	Shader vertShader(vertPath.c_str(), ShaderStage::VertexShader);
	Shader fragShader(fragPath.c_str(), ShaderStage::FragmentShader);

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