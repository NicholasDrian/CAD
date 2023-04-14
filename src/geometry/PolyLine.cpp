#pragma once

#include "PolyLine.h"

PolyLine::PolyLine(const std::vector<glm::vec3>& points, unsigned id)
	: m_Points(points), m_ID(id), m_Color({0.0f, 0.0f, 0.0f}), 
	m_SegmentSelectionBuffer(std::vector<uint32_t>(((points.size() - 1) + 31) / 32, 0U)),
	m_VertexSelectionBuffer(std::vector<uint32_t>((points.size() + 31) / 32, 0U))
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
	if ((m_Points.size() - 1) % 32 == 1) m_SegmentSelectionBuffer.push_back(0U);
	m_Indecies.push_back((unsigned)m_Points.size() - 2U);
	m_Indecies.push_back((unsigned)m_Points.size() - 1U);
	UpdateVertexArray();
}

AxisAlignedBoundingBox PolyLine::GetBoundingBox() const
{
	return AxisAlignedBoundingBox(m_Points);
}


AxisAlignedBoundingBox PolyLine::GetSubSelectionBoundingBox() const
{
	std::vector<glm::vec3> selected;
	bool prev = false;
	for (int i = 0; i < m_Points.size() - 1; i++) {
		int idx = i / 32;
		int shift = i % 32;
		if (m_SegmentSelectionBuffer[idx] & (1 << shift)) {
			selected.push_back(m_Points[i + 1]);
			if (!prev) selected.push_back(m_Points[i]);
			prev = true;
		}
		else {
			prev = false;
		}
	}
	return AxisAlignedBoundingBox(selected);
}

void PolyLine::UpdateLast(const glm::vec3& point) { 
	m_Points.back() = point; 
	UpdateVertexArray();
}

void PolyLine::RemoveLast()
{
	if (m_Points.size() == 0) return;
	m_Points.pop_back();
	if ((m_Points.size() - 1) % 32 == 0) m_SegmentSelectionBuffer.pop_back();
	m_Indecies.pop_back();
	m_Indecies.pop_back();
	UpdateVertexArray();
}

void PolyLine::UpdateVertexArray()
{
	m_VertexArray = std::make_unique<VertexArrayBasicLines>(m_Points, m_Color, m_ID, m_Indecies, 2.0f, true, m_SegmentSelectionBuffer, m_VertexSelectionBuffer);
	// optimize with subdata or some jawn
}

void PolyLine::AddSubSelection(uint32_t subID)
{
	int index = subID / 32;
	int shift = subID % 32;

	if ((m_SegmentSelectionBuffer[index] & (1 << shift)) == 0) {
		m_SegmentSelectionBuffer[index] |= 1 << shift;
		for (int i = 0; i < 2; i++) {
			m_VertexSelectionCounter[subID + i]++;
			index = (subID + i) / 32;
			shift = (subID + i) % 32;
			m_VertexSelectionBuffer[index] |= 1 << shift;
		}
		//m_VertexArray->UpdateSegmentSelectionBuffer(index, m_SegmentSelectionBuffer[index]);
		m_VertexArray->UpdateSegmentSelectionBuffer(m_SegmentSelectionBuffer, m_VertexSelectionBuffer);
	}
}
void PolyLine::RemoveSubSelection(uint32_t subID)
{
	int index = subID / 32;
	int shift = subID % 32;
	if ((m_SegmentSelectionBuffer[index] & (1 << shift)) != 0) {
		m_SegmentSelectionBuffer[index] &= ~(1 << shift);
		for (int i = 0; i < 2; i++) {
			m_VertexSelectionCounter[subID + i]++;
			if (m_VertexSelectionBuffer[subID + i] == 0) {
				index = (subID + i) / 32;
				shift = (subID + i) % 32;
				m_VertexSelectionBuffer[index] &= ~(1 << shift);
			}
		}
		//m_VertexArray->UpdateSegmentSelectionBuffer(index, m_SegmentSelectionBuffer[index]);
		m_VertexArray->UpdateSegmentSelectionBuffer(m_SegmentSelectionBuffer, m_VertexSelectionBuffer);
	}
}
void PolyLine::ClearSubSelection()
{
	for (uint32_t& i : m_SegmentSelectionBuffer) i = 0;
	for (uint32_t& i : m_VertexSelectionBuffer) i = 0;
	m_VertexArray->UpdateSegmentSelectionBuffer(m_SegmentSelectionBuffer, m_VertexSelectionBuffer);
}


