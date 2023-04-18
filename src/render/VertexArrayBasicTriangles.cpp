#pragma once 

#include "VertexArrayBasicTriangles.h"
#include "shaders/ShaderManager.h"

#include<iostream>


VertexArrayBasicTriangles::VertexArrayBasicTriangles(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<uint32_t>& triangleSelectionBuffer, const std::vector<uint32_t>& vertexSelectionBuffer, const glm::vec3& color, uint32_t id, unsigned subIDOffset, const std::vector<unsigned>& indices)
	: m_IndexCount((unsigned)indices.size()), m_Color(color), m_SubIDOffset(subIDOffset)
{
	GLCall(glGenBuffers(1, &m_TriangleSelectionBufferID));
	GLCall(glGenBuffers(1, &m_VertexSelectionBufferID));
	UpdateSelectionBuffers(triangleSelectionBuffer, vertexSelectionBuffer, true);

	GLCall(glGenBuffers(1, &m_VertexBufferID));
	UpdateVertexOrientation(positions, normals);

	GLCall(glGenVertexArrays(1, &m_RenderID));
	GLCall(glBindVertexArray(m_RenderID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));

	GLCall(glGenBuffers(1, &m_IndexBufferID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), indices.data(), GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredTriangleVertex), (void*)offsetof(ColoredTriangleVertex, pos)));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredTriangleVertex), (void*)offsetof(ColoredTriangleVertex, norm)));

	GLCall(glBindVertexArray(0));
}

void VertexArrayBasicTriangles::UpdateVertexOrientation(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals)
{
	std::vector<ColoredTriangleVertex> data;
	for (int i = 0; i < positions.size(); i++) data.emplace_back(positions[i], normals[i]);

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(ColoredTriangleVertex) * data.size(), data.data(), GL_STATIC_DRAW));
}

VertexArrayBasicTriangles::~VertexArrayBasicTriangles() 
{
	GLCall(glDeleteBuffers(1, &m_IndexBufferID));
	GLCall(glDeleteBuffers(1, &m_VertexBufferID));
	GLCall(glDeleteBuffers(1, &m_TriangleSelectionBufferID));
	GLCall(glDeleteBuffers(1, &m_VertexSelectionBufferID));
	GLCall(glDeleteVertexArrays(1, &m_RenderID));

}

void VertexArrayBasicTriangles::Render(const glm::mat4& model, unsigned id, bool selectable, bool subSelectable, bool selected) const {
	ShaderManager::Bind(ShaderProgramType::BasicTriShader);
	ShaderManager::UpdateLocalUniforms(model, m_Color, selectable, subSelectable, selected, id, m_SubIDOffset);
	GLCall(glBindVertexArray(m_RenderID));
	if (subSelectable) {
		GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_TriangleSelectionBufferID));
		GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_VertexSelectionBufferID));
	}
	GLCall(glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, (GLvoid*)0));
}


void VertexArrayBasicTriangles::UpdateSelectionBuffers(const std::vector<uint32_t>& triangleSelection, const std::vector<uint32_t>& vertexSelection, bool updateSize)
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