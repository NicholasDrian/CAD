#pragma once

#include "PolyLine.h"

PolyLine::PolyLine(const std::vector<glm::vec3>& points, unsigned id)
	: m_Points(points), m_ID(id), m_Color({0.0f, 0.0f, 0.0f}), m_SubSelectionBuffer(std::vector<uint32_t>(((points.size() - 1) + 31) / 32, 0U))
{
	for (unsigned i = 0U; i < points.size() - 1; i++) {
		m_Indecies.push_back(i);
		m_Indecies.push_back(i + 1);
	}
	UpdateVertexArray();
}

void PolyLine::Render() const
{
	m_VertexArray->Render(m_ID, m_Selectable, true, m_Selected);
}

void PolyLine::AddPoint(const glm::vec3& point)
{
	m_Points.push_back(point);
	if (m_Points.size() % 32 == 1) m_SubSelectionBuffer.push_back(0U);
	m_Indecies.push_back((unsigned)m_Points.size() - 2U);
	m_Indecies.push_back((unsigned)m_Points.size() - 1U);
	UpdateVertexArray();
}

void PolyLine::UpdateLast(const glm::vec3& point) { 
	m_Points.back() = point; 
	UpdateVertexArray();
}

void PolyLine::RemoveLast()
{
	if (m_Points.size() == 0) return;
	m_Points.pop_back();
	if (m_Points.size() % 32 == 0) m_SubSelectionBuffer.pop_back();
	m_Indecies.pop_back();
	m_Indecies.pop_back();
	UpdateVertexArray();
}

void PolyLine::UpdateVertexArray()
{
	m_VertexArray = std::make_unique<VertexArrayBasicLines>(m_Points, m_Color, m_ID, m_Indecies, 2.0f, true, m_SubSelectionBuffer);
	// optimize with subdata or some jawn
}


