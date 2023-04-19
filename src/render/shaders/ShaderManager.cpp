#pragma once

#include "ShaderManager.h"
#include "../../scene/Scene.h"
#include "../OpenGLUtils.h"

#include <iostream>

void ShaderManager::Init() 
{
	GLCall(glGenBuffers(1, &m_GlobalShaderUniforms));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_GlobalShaderUniforms));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) + sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW));
	GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_GlobalShaderUniforms));

	GLCall(glGenBuffers(1, &m_LocalShaderUniforms));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_LocalShaderUniforms));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) + sizeof(glm::vec4) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t), NULL, GL_DYNAMIC_DRAW));
	GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_LocalShaderUniforms));
}

void ShaderManager::Destroy() {
	for (auto& entry : m_Programs) entry.second.Destroy();
	m_Programs.clear();
}

void ShaderManager::UpdateGlobalUniforms() {
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_GlobalShaderUniforms));
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &Scene::GetCamera()->GetViewProj()[0][0]));
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &Scene::GetSelectionTransform()[0][0]));
}

void ShaderManager::UpdateLocalUniforms(const glm::mat4& model, const glm::vec4& color, bool selectable, bool subSelectable, bool selected, uint32_t id, uint32_t subIDOffset) {
	// TODO, MAKE THIS JUST ONE CALL TO SUB DATA!
	uint32_t data = 0;
	if (selectable)		data |= (1 << 0);
	if (subSelectable)	data |= (1 << 1);
	if (selected)		data |= (1 << 2);
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_LocalShaderUniforms));
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &model[0][0]));
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::vec4), &color[0]));
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) + sizeof(glm::vec4), sizeof(uint32_t), &data));
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) + sizeof(glm::vec4) + sizeof(uint32_t), sizeof(uint32_t), &id));
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) + sizeof(glm::vec4) + sizeof(uint32_t) + sizeof(uint32_t), sizeof(uint32_t), &subIDOffset));
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