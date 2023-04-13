#pragma once 

#include "VertexArrayBasicTriangles.h"
#include "shaders/ShaderManager.h"

#include<iostream>


VertexArrayBasicTriangles::VertexArrayBasicTriangles(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<uint32_t>& triangleSelectionBuffer, const glm::vec3& color, uint32_t id, const std::vector<unsigned>& indices)
	: m_IndexCount((unsigned)indices.size()), m_Model(glm::mat4(1.0)), m_Color(color)
{
	GLCall(glGenBuffers(1, &m_TriangleSelectionBufferID));
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_TriangleSelectionBufferID));
	GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * triangleSelectionBuffer.size(), triangleSelectionBuffer.data(), GL_STATIC_DRAW));
	GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_TriangleSelectionBufferID));
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));


	std::vector<ColoredTriangleVertex> data;
	for (int i = 0; i < positions.size(); i++) {
		data.emplace_back(positions[i], normals[i]); 
	}

	GLCall(glGenVertexArrays(1, &m_RenderID));
	GLCall(glBindVertexArray(m_RenderID));

	GLCall(glGenBuffers(1, &m_VertexBufferID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(ColoredTriangleVertex)* data.size(), data.data(), GL_STATIC_DRAW));

	GLCall(glGenBuffers(1, &m_IndexBufferID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), indices.data(), GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredTriangleVertex), 
		(void*)offsetof(ColoredTriangleVertex, pos)));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredTriangleVertex), 
		(void*)offsetof(ColoredTriangleVertex, norm)));

}

VertexArrayBasicTriangles::~VertexArrayBasicTriangles() 
{
	GLCall(glDeleteBuffers(1, &m_IndexBufferID));
	GLCall(glDeleteBuffers(1, &m_VertexBufferID));
	GLCall(glDeleteBuffers(1, &m_TriangleSelectionBufferID));
	GLCall(glDeleteVertexArrays(1, &m_RenderID));

}

void VertexArrayBasicTriangles::Render(unsigned id, bool selectable, bool subSelectable, bool selected) const {
	ShaderManager::Bind(ShaderProgramType::BasicTriShader);
	ShaderManager::UpdateLocalUniforms(m_Model, m_Color, selectable, subSelectable, selected, id);
	GLCall(glBindVertexArray(m_RenderID));
	if (subSelectable) GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_TriangleSelectionBufferID));
	GLCall(glDrawElements(GL_TRIANGLES, GetIndexCount(), GL_UNSIGNED_INT, (GLvoid*)0));
}

void VertexArrayBasicTriangles::UpdateSegmentSelectionBuffer(unsigned index, uint32_t val)
{
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_TriangleSelectionBufferID));
	GLCall(glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * index, sizeof(uint32_t), &val));
}
void VertexArrayBasicTriangles::UpdateSegmentSelectionBuffer(std::vector<uint32_t> data, bool updateSize)
{
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_TriangleSelectionBufferID));
	if (updateSize) GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * data.size(), data.data(), GL_STATIC_DRAW));
	else			GLCall(glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(uint32_t) * data.size(), data.data()));
}