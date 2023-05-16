#pragma once

#include "PolyLine.h"

#include "../debug/Print.h"
#include "../render/Renderer.h"

PolyLine::PolyLine(const std::vector<glm::vec3>& points, bool dashed, unsigned id)
	: m_Points(points), m_ID(id), m_Color({0.0f, 0.0f, 0.0f, 1.0f}), m_Model(1.0),
	m_SegmentSelectionBuffer(std::vector<uint32_t>(((points.size() - 1) + 31) / 32, 0U)),
	m_VertexSelectionBuffer(std::vector<uint32_t>((points.size() + 31) / 32, 0U)), m_Dashed(dashed), 
	m_Selected(false), m_PointsOn(false)
{
	for (unsigned i = 0U; i < points.size() - 1; i++) {
		m_Indices.push_back(i);
		m_Indices.push_back(i + 1);
	}
	UpdateVertexArrays();
}

void PolyLine::Render() const
{
	if (!m_Selectable) Renderer::UnbindIDBuffer();
	m_VertexArrayLines->Render(m_Model, m_ID, m_Selectable, m_Selected);
	if (m_PointsOn) m_VertexArrayPoints->Render(m_Model, m_ID, m_Selectable, true, m_Selected);
	Renderer::BindIDBuffer();
}

void PolyLine::BakeSelectionTransform(const glm::mat4& t)
{
	if (m_Selected) m_Model = t * m_Model;
	else {
		glm::mat4 tObj = glm::inverse(m_Model) * t * m_Model;
		for (auto e : m_VertexSelectionCounter) {
			m_Points[e.first] = tObj * glm::vec4(m_Points[e.first], 1.0);
		}
		UpdatePositions();
	}
}

std::vector<glm::vec3> PolyLine::GetControlPoints(bool withSelectionTransform)
{
	if (!withSelectionTransform || (!m_Selected && m_VertexSelectionCounter.empty()))
	{
		return m_Points;
	}
	else
	{
		glm::mat4 t = glm::inverse(m_Model) * Scene::GetSelectionTransform() * m_Model;
		if (m_Selected) {
			std::vector<glm::vec3> points(m_Points.size());
			for (int i = 0; i < points.size(); i++) {
				points[i] = t * glm::vec4(m_Points[i], 1.0);
			}
			return points;
		} else {
			std::vector<glm::vec3> points = m_Points;
			for (auto e : m_VertexSelectionCounter) {
				points[e.first] = t * glm::vec4(m_Points[e.first], 1.0);
			}
			return points;
		}
	}
}

void PolyLine::AddPoint(const glm::vec3& point)
{
	m_Points.push_back(point);
	if ((m_Points.size() - 1) % 32 == 1) m_SegmentSelectionBuffer.push_back(0U);
	if ((m_Points.size())	  % 32 == 1) m_VertexSelectionBuffer.push_back(0U);
	m_Indices.push_back((unsigned)m_Points.size() - 2U);
	m_Indices.push_back((unsigned)m_Points.size() - 1U);
	UpdateVertexArrays();
}

AxisAlignedBoundingBox PolyLine::GetBoundingBox() const
{
	return AxisAlignedBoundingBox(m_Points, m_Model);
}

AxisAlignedBoundingBox PolyLine::GetBoundingBoxLocalSpace(uint32_t subID) const
{
	return AxisAlignedBoundingBox({ m_Points[m_Indices[subID * 2]] , m_Points[m_Indices[subID * 2 + 1]] });
}

AxisAlignedBoundingBox PolyLine::GetSubSelectionBoundingBox() const
{
	std::vector<glm::vec3> selected;
	for (int i = 0; i < m_Points.size(); i++) {
		int idx = i / 32;
		int shift = i % 32;
		if (m_VertexSelectionBuffer[idx] & (1 << shift)) {
			selected.push_back(m_Points[i]);
		}
	}
	return AxisAlignedBoundingBox(selected, m_Model);
}

void PolyLine::SelectWithinFrustum(const Frustum& frustum, bool inclusive)
{
	if (m_Selected) return;

	AxisAlignedBoundingBox bb = GetBoundingBox();

	if (bb.FullyWithin(frustum)) {
		m_Selected = true;
		return;
	}
	if (inclusive) 
	{
		for (int i = 0; i < m_Indices.size() / 2; i++) {
			if (frustum.PartiallyContainsLine(
				m_Model * glm::vec4(m_Points[m_Indices[2 * i]], 1.0), 
				m_Model * glm::vec4(m_Points[m_Indices[2 * i + 1]], 1.0))) 
			{
				ClearSubSelection();
				m_Selected = true;
				return;
			}
		}
	} 
	else 
	{
		for (const auto& p : m_Points) {
			if (!frustum.Contains(m_Model * glm::vec4(p, 1.0))) return;
		}
		ClearSubSelection();
		m_Selected = true;
	}
}

void PolyLine::SubSelectWithinFrustum(const Frustum& frustum, bool inclusive)
{
	if (m_Selected) return;
	AxisAlignedBoundingBox bb = GetBoundingBox();
	if (bb.FullyWithin(frustum)) {
		for (int i = 0; i < m_Indices.size() / 2; i++) {
			AddSubSelectionLine(i);
		}
		for (int i = 0; i < m_Points.size(); i++) {
			AddSubSelectionPoint(i);
		}
	}
	else {
		if (m_PointsOn) {
			for (int i = 0; i < m_Points.size(); i++) {
				if (frustum.Contains(m_Model * glm::vec4(m_Points[i], 1.0)))
					AddSubSelectionPoint(i);
			}
		}
		if (inclusive) {
			for (int i = 0; i < m_Indices.size() / 2; i++) {
				if (frustum.PartiallyContainsLine(
					m_Model * glm::vec4(m_Points[m_Indices[2 * i]], 1.0),
					m_Model * glm::vec4(m_Points[m_Indices[2 * i + 1]], 1.0)))
				{
					AddSubSelectionLine(i);
				}
			}
		}
		else {
			for (int i = 0; i < m_Indices.size() / 2; i++) {
				if (frustum.Contains(m_Model * glm::vec4(m_Points[m_Indices[2 * i]], 1.0)) &&
					frustum.Contains(m_Model * glm::vec4(m_Points[m_Indices[2 * i + 1]], 1.0)))
				{
					AddSubSelectionLine(i);
				}
			}
		}
	}

}

void PolyLine::UnSelectWithinFrustum(const Frustum& frustum, bool inclusive)
{
	if (!m_Selected) return;
	if (GetBoundingBox().FullyWithin(frustum)) {
		m_Selected = false;
	}
	else if (inclusive) {
		for (int i = 0; i < m_Indices.size() / 2; i++) {
			if (frustum.PartiallyContainsLine(
					m_Model * glm::vec4(m_Points[m_Indices[i * 2]], 1.0), 
					m_Model * glm::vec4(m_Points[m_Indices[i * 2 * 1]], 1.0))) {
				m_Selected = false;
				break;
			}
		}
	}
	else {
		for (const glm::vec3& point : m_Points) {
			if (!frustum.Contains(m_Model * glm::vec4(point, 1.0))) return;
		}
		m_Selected = false;
	}
}

void PolyLine::UnSubSelectWithinFrustum(const Frustum& frustum, bool inclusive)
{
	if (m_VertexSelectionCounter.empty()) return;
	if (GetBoundingBox().FullyWithin(frustum)) {
		ClearSubSelection();
	}
	else {
		for (int i = 0; i < m_Points.size(); i++) {
			if (frustum.Contains(m_Model * glm::vec4(m_Points[i], 1.0)))
				RemoveSubSelectionPoint(i);
		}
		if (inclusive) {
			for (int i = 0; i < m_Indices.size() / 2; i++) {
				if (frustum.PartiallyContainsLine(
					m_Model * glm::vec4(m_Points[m_Indices[2 * i]], 1.0),
					m_Model * glm::vec4(m_Points[m_Indices[2 * i + 1]], 1.0)))
				{
					RemoveSubSelectionLine(i);
				}
			}
		}
		else {
			for (int i = 0; i < m_Indices.size() / 2; i++) {
				if (frustum.Contains(m_Model * glm::vec4(m_Points[m_Indices[2 * i]], 1.0)) &&
					frustum.Contains(m_Model * glm::vec4(m_Points[m_Indices[2 * i + 1]], 1.0)))
				{
					RemoveSubSelectionLine(i);
				}
			}
		}
	}
}

void PolyLine::UpdateLast(const glm::vec3& point) { 
	m_Points.back() = point; 
	UpdatePositions();
}

void PolyLine::RemoveLast()
{
	if (m_Points.size() == 0) return;
	m_Points.pop_back();
	if ((m_Points.size() - 1) % 32 == 0) m_SegmentSelectionBuffer.pop_back();
	if ((m_Points.size()	) % 32 == 0) m_VertexSelectionBuffer.pop_back();
	m_Indices.pop_back(), m_Indices.pop_back();
	UpdateVertexArrays();
}

void PolyLine::UpdateVertexArrays()
{
	m_VertexArrayLines = std::make_unique<VertexArraySubSelectableLines>(m_Points, m_Color, m_ID, m_Indices, 2.0f, m_Dashed, m_SegmentSelectionBuffer, m_VertexSelectionBuffer);

	m_VertexArrayPoints = std::make_unique<VertexArrayPoints>(m_Points, m_VertexSelectionBuffer, m_Color, (unsigned)GetNumSegments());
}

void PolyLine::UpdatePositions()
{
	m_VertexArrayLines->UpdateVertexPositions(m_Points);
	m_VertexArrayPoints->UpdatePositions(m_Points);
}

void PolyLine::UpdateSubSelections()
{
	m_VertexArrayLines->UpdateSegmentSelectionBuffer(m_SegmentSelectionBuffer, m_VertexSelectionBuffer);
	m_VertexArrayPoints->UpdateSubSelection(m_VertexSelectionBuffer);
}

void PolyLine::AddSubSelection(uint32_t subID)
{
	if (subID < GetNumSegments()) AddSubSelectionLine(subID);
	else AddSubSelectionPoint(subID - (uint32_t)GetNumSegments());
}


void PolyLine::AddSubSelectionLine(uint32_t subID) 
{
	int index = subID / 32;
	int shift = subID % 32;

	if ((m_SegmentSelectionBuffer[index] & (1 << shift)) == 0) {
		m_SegmentSelectionBuffer[index] |= 1 << shift;
		for (int i = 0; i < 2; i++) {
			index = (subID + i) / 32;
			shift = (subID + i) % 32;
			m_VertexSelectionBuffer[index] |= 1 << shift;
			m_VertexSelectionCounter[subID + i]++;
		}
		UpdateSubSelections();
	}
}
void PolyLine::AddSubSelectionPoint(uint32_t subID) 
{
	int index = subID / 32;
	int shift = subID % 32;
	if ((m_VertexSelectionBuffer[index] & (1 << shift)) == 0) {
		m_VertexSelectionBuffer[index] |= 1 << shift;
		m_VertexSelectionCounter[subID]++;
		UpdateSubSelections();
	}
}


void PolyLine::RemoveSubSelection(uint32_t subID)
{
	if (subID < GetNumSegments()) RemoveSubSelectionLine(subID);
	else RemoveSubSelectionPoint(subID - (uint32_t)GetNumSegments());
}


void PolyLine::RemoveSubSelectionLine(uint32_t subID)
{
	int index = subID / 32;
	int shift = subID % 32;
	if ((m_SegmentSelectionBuffer[index] & (1 << shift)) != 0) {
		m_SegmentSelectionBuffer[index] &= ~(1 << shift);
		for (int i = 0; i < 2; i++) {
			m_VertexSelectionCounter[subID + i]--;
			if (m_VertexSelectionCounter[subID + i] == 0) {
				index = (subID + i) / 32;
				shift = (subID + i) % 32;
				m_VertexSelectionBuffer[index] &= ~(1 << shift);
				m_VertexSelectionCounter.erase(subID + i);
			}
		}
		UpdateSubSelections();
	}
}
void PolyLine::RemoveSubSelectionPoint(uint32_t subID)
{
	int index = subID / 32;
	int shift = subID % 32;
	if ((m_VertexSelectionBuffer[index] & (1 << shift)) != 0) {
		m_VertexSelectionBuffer[index] &= ~(1 << shift);
		m_VertexSelectionCounter[subID]--;
		UpdateSubSelections();
	}
}

void PolyLine::ClearSubSelection()
{
	for (uint32_t& i : m_SegmentSelectionBuffer) i = 0;
	for (uint32_t& i : m_VertexSelectionBuffer) i = 0;
	m_VertexSelectionCounter.clear();
	UpdateSubSelections();
}

glm::vec3 PolyLine::Intersect(Ray r, uint32_t subID) const
{
	glm::vec3 p1 = m_Model * glm::vec4(m_Points[m_Indices[subID * 2]], 1.0f);
	glm::vec3 p2 = m_Model * glm::vec4(m_Points[m_Indices[subID * 2 + 1]], 1.0f);
	return r.ClosestPointOnLine(p1, p2);
}

bool PolyLine::IntersectsLocalSpace(Ray r, uint32_t subID, float& outT, float MaxDistancePixels) const
{
	glm::vec3 p1 = m_Points[m_Indices[subID * 2]];
	glm::vec3 p2 = m_Points[m_Indices[subID * 2 + 1]];
	float dist;
	glm::vec3 intersection = r.ClosestPointOnLine(p1, p2, outT, dist);
	float pixelSize = Scene::GetCamera()->GetPixelSizeAtPoint(intersection);
	return dist / pixelSize >= MaxDistancePixels;
}
