#pragma once

#include "ShaderProgram.h"
#include <stdexcept>
#include "../OpenGLUtils.h"

#include <iostream>

const std::string SHADER_BASE_PATH("src/render/shaders/shaders/");

ShaderProgram::ShaderProgram(ShaderProgramType type)
{
	std::string vertPath;
	std::string fragPath;

	switch (type) {
	case ShaderProgramType::TriShader:
		vertPath = SHADER_BASE_PATH + "BasicTri.vert";
		fragPath = SHADER_BASE_PATH + "BasicTri.frag";
		break;
	case ShaderProgramType::LineShader:
		vertPath = SHADER_BASE_PATH + "BasicLine.vert";
		fragPath = SHADER_BASE_PATH + "BasicLine.frag";
		break;
	case ShaderProgramType::PointShader:
		vertPath = SHADER_BASE_PATH + "BasicPoint.vert";
		fragPath = SHADER_BASE_PATH + "BasicPoint.frag";
		break;
	case ShaderProgramType::ScreenSpaceTriShader:
		vertPath = SHADER_BASE_PATH + "ScreenSpaceTri.vert";
		fragPath = SHADER_BASE_PATH + "ScreenSpaceTri.frag";
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

}