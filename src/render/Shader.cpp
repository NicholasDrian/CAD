#pragma once

#include "Shader.h"

#include "../utils/OpenGLUtils.h"

#include <fstream>
#include <sstream>
#include <string>

#include <iostream>

Shader::Shader(const char* path, ShaderType type) {
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
	//std::cout << m_ID << '\n';
	m_ID = GLResult(glCreateShader(GetGLShaderType(type)));
	//std::cout << m_ID << "\n\n";
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
	//throw std::runtime_error("rsdsvasd");
	GLCall(glDeleteShader(m_ID));
}

GLuint Shader::GetID() const
{
	return m_ID;
}

GLenum Shader::GetGLShaderType(ShaderType type)
{
	switch (type) {
	case ShaderType::VertexShader:
		return GL_VERTEX_SHADER;
	case ShaderType::FragmentShader:
		return GL_FRAGMENT_SHADER;
	default:
		throw std::runtime_error("Unimplemented Shader Type Error");
	}
}
