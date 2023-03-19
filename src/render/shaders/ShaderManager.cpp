#pragma once

#include "ShaderManager.h"
#include "ColoredTriShader.h"
#include "ColoredLineShader.h"
#include "TexturedShader.h"
#include "../../scene/Scene.h"
#include "../OpenGLUtils.h"

void ShaderManager::Init() 
{
	GLCall(glGenBuffers(1, &m_GlobalShaderUniforms));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_GlobalShaderUniforms));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, sizeof(Scene::GetCamera()->GetViewProj()), NULL, GL_DYNAMIC_DRAW));
}

void ShaderManager::Destroy() {
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
		case ShaderProgramType::ColoredTriShader:
			m_Programs[type] = ColoredTriShader();
			break;
		case ShaderProgramType::ColoredLineShader:
			m_Programs[type] = ColoredLineShader();
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