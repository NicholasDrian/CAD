#pragma once

#include "Mesh.h"
#include "../render/Renderer.h"

Mesh::Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const glm::vec3& color, const std::vector<unsigned>& indecies, unsigned id, bool selectable)
	: m_Positions(positions), m_Normals(normals), m_Color(color), m_ID(id), m_Indecies(indecies), m_VertexArray(VertexArrayBasicTriangles(positions, normals, color, m_ID, indecies))
{
	m_Selectable = selectable;
}

void Mesh::Render() const {
	m_VertexArray.Render(m_ID, m_Selectable, m_Selected);
}