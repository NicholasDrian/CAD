#pragma once

#include "ShaderManager.h"
#include "ColoredShader.h"
#include "TexturedShader.h"
#include "../../scene/Scene.h"
#include "../OpenGLUtils.h"

ShaderManager::ShaderManager()
	: m_BoundProgram(ShaderProgramType::None)
{
}

void ShaderManager::Init() 
{
	GLCall(glGenBuffers(1, &m_GlobalShaderUniforms));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_GlobalShaderUniforms));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, sizeof(Scene::GetCamera()->GetViewProj()), NULL, GL_DYNAMIC_DRAW));
}

ShaderManager::~ShaderManager() 
{
	for (auto& entry : m_Programs) entry.second.Destroy();
	m_Programs.clear();
}

void ShaderManager::UpdateViewProj() {
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_GlobalShaderUniforms));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, sizeof(Scene::GetCamera()->GetViewProj()), &Scene::GetCamera()->GetViewProj()[0][0], GL_DYNAMIC_DRAW));
}

void ShaderManager::Bind(ShaderProgramType type) 
{
	if (m_BoundProgram == type) return;
	if (!m_Programs.contains(type)) 
	{
		switch (type) {
		case ShaderProgramType::ColoredShader:
			m_Programs[type] = ColoredShader();
			break;
		case ShaderProgramType::TexturedShader:
			m_Programs[type] = TexturedShader();
			break;
		}
		m_Programs[type].Bind();
		GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_GlobalShaderUniforms));
	}
	else 
	{
		m_Programs[type].Bind();
	}
	m_BoundProgram = type;
}