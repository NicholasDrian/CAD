#pragma once

#include "Mesh.h"
#include "../render/Renderer.h"

Mesh::Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const glm::vec3& color, const std::vector<unsigned>& indices, unsigned id)
	: m_Positions(positions), m_Normals(normals), m_Color(color), m_ID(id), m_Indices(indices), m_SelectedTriangles(std::vector<uint32_t>(((indices.size() / 3) + 31) / 32, 0U))
{
	m_VertexArray = std::make_unique<VertexArrayBasicTriangles>(m_Positions, m_Normals, m_SelectedTriangles, m_Color, m_ID, m_Indices);
}

void Mesh::AddSubSelection(uint32_t subID)
{
	int index = subID / 32;
	int shift = subID % 32;
	m_SelectedTriangles[index] |= 1 << shift;
	m_VertexArray->UpdateSegmentSelectionBuffer(index, m_SelectedTriangles[index]);
}
void Mesh::RemoveSubSelection(uint32_t subID)
{
	int index = subID / 32;
	int shift = subID % 32;
	m_SelectedTriangles[index] &= ~(1 << shift);
	m_VertexArray->UpdateSegmentSelectionBuffer(index, m_SelectedTriangles[index]);
}
void Mesh::ClearSubSelection()
{
	for (uint32_t& i : m_SelectedTriangles) i = 0;
	m_VertexArray->UpdateSegmentSelectionBuffer(m_SelectedTriangles);
}

void Mesh::Render() const {
	m_VertexArray->Render(m_ID, m_Selectable, true, m_Selected);
}