#pragma once

#include "Shader.h"

#include "../OpenGLUtils.h"

#include <fstream>
#include <sstream>
#include <string>

#include <iostream>

Shader::Shader(const char* path, ShaderStage type) {
	std::string source;
	std::string line;
	std::ifstream file(path);
	if (file.is_open())
	{
		while (std::getline(file, line)) source += line + '\n';
		file.close();
	}
	else 
	{
		throw std::runtime_error("failed to open shader file!");
	}
	const char* cString = source.c_str();
	m_ID = GLResult(glCreateShader(GetGLShaderType(type)));
	GLCall(glShaderSource(m_ID, 1, &cString, NULL));
	GLCall(glCompileShader(m_ID));

#ifdef CAD_DEBUG
	GLint compiled;
	GLCall(glGetShaderiv(m_ID, GL_COMPILE_STATUS, &compiled));
	if (compiled != GL_TRUE) PrintShaderInfoLog(m_ID);
#endif
}

Shader::~Shader()
{
	GLCall(glDeleteShader(m_ID));
}

GLuint Shader::GetID() const
{
	return m_ID;
}

GLenum Shader::GetGLShaderType(ShaderStage type)
{
	switch (type) {
	case ShaderStage::VertexShader:
		return GL_VERTEX_SHADER;
	case ShaderStage::FragmentShader:
		return GL_FRAGMENT_SHADER;
	default:
		throw std::runtime_error("Unimplemented Shader Type Error");
	}
}
