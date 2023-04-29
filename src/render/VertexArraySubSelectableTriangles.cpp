#pragma once

#include "VertexArraySubSelectableTriangles.h"
#include "shaders/ShaderManager.h"

#include <iostream>


VertexArraySubSelectableTriangles::VertexArraySubSelectableTriangles(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<uint32_t>& triangleSelectionBuffer, const std::vector<uint32_t>& vertexSelectionBuffer, const glm::vec4& color, uint32_t id, const std::vector<unsigned>& indices)
	: VertexArrayTriangles(positions, normals, color, indices)
{
	GLCall(glGenBuffers(1, &m_TriangleSelectionBufferID));
	GLCall(glGenBuffers(1, &m_VertexSelectionBufferID));
	UpdateSelectionBuffers(triangleSelectionBuffer, vertexSelectionBuffer, true);
}

VertexArraySubSelectableTriangles::~VertexArraySubSelectableTriangles()
{
	VertexArrayTriangles::~VertexArrayTriangles();
	GLCall(glDeleteBuffers(1, &m_TriangleSelectionBufferID));
	GLCall(glDeleteBuffers(1, &m_VertexSelectionBufferID));
}

void VertexArraySubSelectableTriangles::Render(const glm::mat4& t, unsigned id, bool selectable, bool selected) const
{
	ShaderManager::Bind(ShaderProgramType::TriShader);
	ShaderManager::UpdateLocalUniforms(t, m_Color, selectable, true, selected, id);
	GLCall(glBindVertexArray(m_RenderID));
	GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_TriangleSelectionBufferID));
	GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_VertexSelectionBufferID));
	GLCall(glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, (GLvoid*)0));
}

void VertexArraySubSelectableTriangles::UpdateSelectionBuffers(const std::vector<uint32_t>& triangleSelection, const std::vector<uint32_t>& vertexSelection, bool updateSize)
{
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_TriangleSelectionBufferID));
	if (updateSize)
		GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * triangleSelection.size(), triangleSelection.data(), GL_STATIC_DRAW));
	else
		GLCall(glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(uint32_t) * triangleSelection.size(), triangleSelection.data()));

	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_VertexSelectionBufferID));
	if (updateSize)
		GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * vertexSelection.size(), vertexSelection.data(), GL_STATIC_DRAW));
	else
		GLCall(glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(uint32_t) * vertexSelection.size(), vertexSelection.data()));
}
