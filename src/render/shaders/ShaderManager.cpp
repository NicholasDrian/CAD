#pragma once

#include "ShaderManager.h"
#include "../../scene/Scene.h"
#include "../OpenGLUtils.h"

#include <iostream>

void ShaderManager::Init() 
{
	GLCall(glGenBuffers(1, &m_GlobalShaderUniforms));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_GlobalShaderUniforms));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, sizeof(Scene::GetCamera()->GetViewProj()), NULL, GL_DYNAMIC_DRAW));
	GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_GlobalShaderUniforms));

	GLCall(glGenBuffers(1, &m_LocalShaderUniforms));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_LocalShaderUniforms));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) + 4 + 4, NULL, GL_DYNAMIC_DRAW));
	GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_LocalShaderUniforms));

}

void ShaderManager::Destroy() {
	for (auto& entry : m_Programs) entry.second.Destroy();
	m_Programs.clear();
}

void ShaderManager::UpdateGlobalUniforms() {
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_GlobalShaderUniforms));
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &Scene::GetCamera()->GetViewProj()[0][0]));
}

void ShaderManager::UpdateLocalUniforms(const glm::mat4& model, bool selectable, bool selected, unsigned id) {
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_LocalShaderUniforms));
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &model[0][0]));

	int data = 0;
	if (selectable) data |= (1 << 0);
	if (selected)	data |= (1 << 1);
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(int), &data));

	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) + sizeof(int), sizeof(unsigned), &id));
}

void ShaderManager::Bind(ShaderProgramType type) 
{
	if (m_BoundProgram == type) return;
	if (!m_Programs.contains(type)) 
	{
		m_Programs[type] = ShaderProgram(type);
	}
	m_Programs[type].Bind();
	m_BoundProgram = type;
}