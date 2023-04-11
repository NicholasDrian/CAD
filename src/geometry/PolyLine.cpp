#pragma once

#include "PolyLine.h"

PolyLine::PolyLine(const std::vector<glm::vec3>& points, unsigned id)
	: m_Points(points), m_ID(id), m_Color({0.0f, 0.0f, 0.0f})
{
	for (unsigned i = 0U; i < points.size() - 1; i++) {
		m_Indecies.push_back(i);
		m_Indecies.push_back(i + 1);
	}
	UpdateVertexArray();
}

void PolyLine::Render() const
{
	m_VertexArray->Render(m_ID, m_Selectable, m_Selected);
}

void PolyLine::AddPoint(const glm::vec3& point)
{
	m_Points.push_back(point);
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
	m_Indecies.pop_back();
	m_Indecies.pop_back();
	UpdateVertexArray();
}

void PolyLine::UpdateVertexArray()
{
	m_VertexArray = std::make_unique<VertexArrayBasicLines>(m_Points, m_Color, m_ID, m_Indecies, 2.0f);
	// optimize with subdata or some jawn
}


