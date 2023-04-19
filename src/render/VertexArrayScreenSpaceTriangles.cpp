#pragma once

#include "VertexArrayScreenSpaceTriangles.h"

#include "shaders/ShaderManager.h"
#include "Renderer.h"

#include <iostream>

VertexArrayScreenSpaceTriangles::VertexArrayScreenSpaceTriangles(const std::vector<glm::vec3>& positions, const std::vector<uint32_t>& indices, const glm::vec4& color)
	: m_Color(color), m_IndexCount(indices.size())
{
	GLCall(glGenVertexArrays(1, &m_ID));
	GLCall(glGenBuffers(1, &m_VertexBufferID));
	GLCall(glGenBuffers(1, &m_IndexBufferID));

	GLCall(glBindVertexArray(m_ID));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0));

	GLCall(glBindVertexArray(0));

}

VertexArrayScreenSpaceTriangles::~VertexArrayScreenSpaceTriangles()
{
	GLCall(glDeleteBuffers(1, &m_VertexBufferID));
	GLCall(glDeleteBuffers(1, &m_IndexBufferID));
	GLCall(glDeleteVertexArrays(1, &m_ID));
}

void VertexArrayScreenSpaceTriangles::Render() const
{
	Renderer::UnbindIDBuffer();
	Renderer::DisableDepthTesting();

	ShaderManager::Bind(ShaderProgramType::ScreenSpaceTriShader);
	ShaderManager::UpdateLocalUniforms(glm::mat4(), m_Color, false, false, false, 0, 0);

	GLCall(glBindVertexArray(m_ID));
	GLCall(glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, (GLvoid*)0));

	Renderer::EnableDepthTesting();
	Renderer::BindIDBuffer();
}
