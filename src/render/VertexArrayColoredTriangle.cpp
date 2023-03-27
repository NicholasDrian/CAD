#pragma once 

#include "VertexArrayColoredTriangle.h"
#include "shaders/ShaderManager.h"

#include<iostream>


ColoredTriangleVertex::ColoredTriangleVertex(glm::vec3 Pos, glm::vec3 Norm, glm::vec3 Col, uint32_t I)
	: pos(Pos), norm(Norm), col(Col), i(I) { }


VertexArrayColoredTriangle::VertexArrayColoredTriangle(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<glm::vec3>& colors, uint32_t id, const std::vector<unsigned>& indices, bool selectable)
	: m_IndexCount(indices.size()), m_Selectable(selectable), m_Model(glm::mat4(1.0)), m_Selected(false)
{

	std::vector<ColoredTriangleVertex> data;
	for (int i = 0; i < positions.size(); i++) {
		data.emplace_back(positions[i], normals[i], colors[i], id); 
		//  TODO: id is a waste of space, should move to uniform and set durring bind
	}

	GLCall(glGenBuffers(1, &m_VertexBufferID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(ColoredTriangleVertex)* data.size(), data.data(), GL_STATIC_DRAW));

	GLCall(glGenBuffers(1, &m_IndexBufferID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), indices.data(), GL_STATIC_DRAW));

	GLCall(glGenVertexArrays(1, &m_ID));
	GLCall(glBindVertexArray(m_ID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glEnableVertexAttribArray(3));

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredTriangleVertex), 
		(void*)offsetof(ColoredTriangleVertex, pos)));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredTriangleVertex), 
		(void*)offsetof(ColoredTriangleVertex, norm)));
	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredTriangleVertex), 
		(void*)offsetof(ColoredTriangleVertex, col)));
	GLCall(glVertexAttribIPointer(3, 1, GL_INT, sizeof(ColoredTriangleVertex), 
		(void*)offsetof(ColoredTriangleVertex, i)));



}

VertexArrayColoredTriangle::~VertexArrayColoredTriangle() 
{
	GLCall(glDeleteBuffers(1, &m_IndexBufferID));
	GLCall(glDeleteBuffers(1, &m_VertexBufferID));
	GLCall(glDeleteVertexArrays(1, &m_ID));

}

void VertexArrayColoredTriangle::Render() const {
	ShaderManager::Bind(ShaderProgramType::ColoredTriShader);
	ShaderManager::UpdateLocalUniforms(m_Model, m_Selectable, m_Selected);
	GLCall(glBindVertexArray(m_ID));
	GLCall(glDrawElements(GL_TRIANGLES, GetIndexCount(), GL_UNSIGNED_INT, (GLvoid*)0));
}