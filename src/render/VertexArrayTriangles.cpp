#pragma once 

#include "VertexArrayTriangles.h"
#include "shaders/ShaderManager.h"

#include<iostream>


VertexArrayTriangles::VertexArrayTriangles(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const glm::vec4& color, const std::vector<unsigned>& indices)
	: m_IndexCount((unsigned)indices.size()), m_Color(color)
{
	
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

void VertexArrayTriangles::UpdateVertexOrientation(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals)
{
	std::vector<ColoredTriangleVertex> data;
	for (int i = 0; i < positions.size(); i++) data.emplace_back(positions[i], normals[i]);

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(ColoredTriangleVertex) * data.size(), data.data(), GL_STATIC_DRAW));
}

VertexArrayTriangles::~VertexArrayTriangles() 
{
	GLCall(glDeleteBuffers(1, &m_IndexBufferID));
	GLCall(glDeleteBuffers(1, &m_VertexBufferID));
	GLCall(glDeleteVertexArrays(1, &m_RenderID));

}

void VertexArrayTriangles::Render(const glm::mat4& model, unsigned id, bool selectable, bool selected) const {
	ShaderManager::Bind(ShaderProgramType::TriShader);
	ShaderManager::UpdateLocalUniforms(model, m_Color, selectable, false, selected, id);
	GLCall(glBindVertexArray(m_RenderID));	
	GLCall(glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, (GLvoid*)0));
}