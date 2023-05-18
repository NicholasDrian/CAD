#pragma once

#include "Mesh.h"
#include "../render/Renderer.h"

Mesh::Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const glm::vec4& color, const std::vector<unsigned>& indices, unsigned id)
	: m_Positions(positions), m_Normals(normals), m_Color(color), m_ID(id), m_Indices(indices), m_Model(1.0),
	m_SelectedTriangles(std::vector<uint32_t>(((indices.size() / 3) + 31) / 32, 0U)),
	m_SelectedVertices(std::vector<uint32_t>(((m_Positions.size()) + 31) / 32, 0U)), m_Selected(false)
{
	m_VertexArray = std::make_unique<VertexArraySubSelectableTriangles>(m_Positions, m_Normals, m_SelectedTriangles, m_SelectedVertices, m_Color, m_ID, m_Indices);
}

AxisAlignedBoundingBox Mesh::GetBoundingBox() const
{
	return AxisAlignedBoundingBox(m_Positions, m_Model);
}

AxisAlignedBoundingBox Mesh::GetBoundingBoxLocalSpace(uint32_t subID) const
{
	glm::vec3 p1 = m_Positions[m_Indices[subID * 3]];
	glm::vec3 p2 = m_Positions[m_Indices[subID * 3 + 1]];
	glm::vec3 p3 = m_Positions[m_Indices[subID * 3 + 2]];
	return AxisAlignedBoundingBox({p1, p2, p3});
}

AxisAlignedBoundingBox Mesh::GetSubSelectionBoundingBox() const
{
	std::vector<glm::vec3> selectedVerts;
	for (const auto& e : m_SelectedVertexCounter) selectedVerts.push_back(m_Positions[e.first]);
	return AxisAlignedBoundingBox(selectedVerts, m_Model);
}

void Mesh::BakeSelectionTransform(const glm::mat4& t)
{
	if (m_Selected) m_Model = t * m_Model;
	else {
		const glm::mat4 tObj = glm::inverse(m_Model) * t * m_Model;
		for (auto e : m_SelectedVertexCounter) {
			m_Positions[e.first] = tObj * glm::vec4(m_Positions[e.first], 1.0);
			m_Normals[e.first] = tObj * glm::vec4(m_Normals[e.first], 0.0);
		}
		m_VertexArray->UpdateVertexOrientation(m_Positions, m_Normals);
	}
}

void Mesh::AddSubSelection(uint32_t subID)
{
	int index = subID / 32;
	int shift = subID % 32;
	if ((m_SelectedTriangles[index] & (1 << shift)) == 0) {
		m_SelectedTriangles[index] |= 1 << shift;
		for (int i = 0; i < 3; i++)
		{
			int vertIdx = m_Indices[3 * subID + i];
			m_SelectedVertexCounter[vertIdx]++;
			index = vertIdx / 32;
			shift = vertIdx % 32;
			m_SelectedVertices[index] |= 1 << shift;
		}
		m_VertexArray->UpdateSelectionBuffers(m_SelectedTriangles, m_SelectedVertices);
	}
}
void Mesh::RemoveSubSelection(uint32_t subID)
{
	int index = subID / 32;
	int shift = subID % 32;
	if ((m_SelectedTriangles[index] & (1 << shift)) != 0) {
		m_SelectedTriangles[index] &= ~(1 << shift);
		for (int i = 0; i < 3; i++) {
			int vertIdx = m_Indices[3 * subID + i];
			m_SelectedVertexCounter[vertIdx]--;
			if (m_SelectedVertexCounter[vertIdx] == 0) {
				index = vertIdx / 32;
				shift = vertIdx % 32;
				m_SelectedVertices[index] &= ~(1 << shift);
				m_SelectedVertexCounter.erase(index);
			}
		}
		m_VertexArray->UpdateSelectionBuffers(m_SelectedTriangles, m_SelectedVertices);
	}
}
void Mesh::ClearSubSelection()
{
	for (uint32_t& i : m_SelectedTriangles) i = 0;
	for (uint32_t& i : m_SelectedVertices) i = 0;
	m_SelectedVertexCounter.clear();
	m_VertexArray->UpdateSelectionBuffers(m_SelectedTriangles, m_SelectedVertices);
}

void Mesh::Render() const {
	if (!m_Selectable) Renderer::UnbindIDBuffer();
	m_VertexArray->Render(m_Model, m_ID, m_Selectable, m_Selected);
	Renderer::BindIDBuffer();
}

glm::vec3 Mesh::Intersect(Ray r, uint32_t subID) const
{
	glm::vec3 point;
	r.IntersectPlane(
		m_Model * glm::vec4(m_Positions[m_Indices[subID * 3]], 1.0f),
		m_Model * glm::vec4(m_Positions[m_Indices[subID * 3 + 1]], 1.0f),
		m_Model * glm::vec4(m_Positions[m_Indices[subID * 3 + 2]], 1.0f), point, false);
	return point;
}

bool Mesh::IntersectsLocalSpace(Ray r, uint32_t subID, float& outT, float MaxDistancePixels) const
{
	return r.IntersectPlane(
		m_Positions[m_Indices[subID * 3]],
		m_Positions[m_Indices[subID * 3 + 1]],
		m_Positions[m_Indices[subID * 3 + 2]], outT, false);
}

bool Mesh::Intersect(Ray r, float& outT) const
{
	// todo
	return false;
}
