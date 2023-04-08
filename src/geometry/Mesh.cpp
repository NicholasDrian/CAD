#pragma once

#include "Mesh.h"
#include "../render/Renderer.h"

Mesh::Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<glm::vec3>& colors, const std::vector<unsigned>& indecies, unsigned id, bool selectable)
	: m_Positions(positions), m_Normals(normals), m_Colors(colors), m_ID(id), m_Indecies(indecies), m_VertexArray(VertexArrayColoredTriangle(positions, normals, colors, m_ID, indecies))
{
	m_Selectable = selectable;
}

void Mesh::Render() const {
	m_VertexArray.Render(m_ID, m_Selectable, m_Selected);
}