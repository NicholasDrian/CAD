#pragma once

#include "NURBS.h"

const unsigned SAMPLES_PER_EDGE = 20;

NURBS::NURBS(std::vector<glm::vec3> points, glm::vec3 color, std::vector<float> weights, unsigned degree, std::vector<float> knots, unsigned id)
	: m_Degree(degree), m_Knots(knots), m_ID(id), m_Color(color), m_Model(1.0f)
{
	if (weights.size() == 0) 
	{
		for (const glm::vec3& point : points) m_Points.emplace_back(point.x, point.y, point.z, 1.0f);
	}
	else 
	{
		for (int i = 0; i < points.size(); i++) {
			const glm::vec3& point = points[i];
			m_Points.emplace_back(point.x * weights[i], point.y * weights[i], point.z * weights[i], weights[i]);
		}
	}
	while (m_Degree > m_Points.size() - 1) m_Degree--;
	UpdateKnotVector();
	UpdateSamples();
	UpdateVertexArray();
}
	
void NURBS::Render() const
{
	m_VertexArray->Render(m_Model, m_ID, m_Selectable, false, m_Selected);
}

void NURBS::BakeSelectionTransform(const glm::mat4& t)
{
	if (m_Selected) m_Model = t * m_Model;
	else {	
		// TODO: CHECK - i think its fine to transform homogenious points
	}
}

AxisAlignedBoundingBox NURBS::GetBoundingBox() const
{
	std::vector<glm::vec3> points(m_Points.size());
	for (const glm::vec4& point : m_Points) points.emplace_back(point.x, point.y, point.z);
	return AxisAlignedBoundingBox(points, m_Model);
}


AxisAlignedBoundingBox NURBS::GetSubSelectionBoundingBox() const
{
	//todo
	return AxisAlignedBoundingBox();
}

void NURBS::AddControlPoint(const glm::vec3& point, bool incrementDegree)
{
	m_Points.emplace_back(point.x, point.y, point.z, 1.0f);
	m_Degree += incrementDegree;
	UpdateKnotVector();
	UpdateSamples();
	UpdateVertexArray();
}

void NURBS::ChangeDegree(unsigned degree)
{
	while (degree >= m_Points.size()) degree--;
	m_Degree = degree;
	UpdateKnotVector();
	UpdateSamples();
	UpdateVertexArray();
}

void NURBS::UpdateLastPoint(const glm::vec3& point)
{
	m_Points.back() = {point.x, point.y, point.z, 1.0f};
	UpdateSamples(); // shouldnt need to update all samples
	m_VertexArray->UpdateVertexPositions(m_Samples);
}

void NURBS::RemoveLastPoint()
{
	m_Points.pop_back();
	if (m_Degree > m_Points.size() - 1) m_Degree--;
	UpdateKnotVector();
	UpdateSamples();
	UpdateVertexArray();
}

std::vector<float> NURBS::BasisFuncs(float u) const
{
	int i = KnotSpan(u);
	std::vector<float> res(m_Degree + 1);
	res[0] = 1.0;
	auto left = [&](int j) { return u - m_Knots[i - j + 1]; };
	auto right = [&](int j) { return m_Knots[i + j] - u; };
	for (unsigned j = 1; j <= m_Degree; j++) {
		float saved = 0.0;
		for (unsigned r = 0; r < j; r++) {
			float temp = res[r] / (right(r + 1U) + left(j - r));
			res[r] = saved + right(r + 1U) * temp;
			saved = left(j - r) * temp;
		}
		res[j] = saved;
	}
	return res;
}

int NURBS::KnotSpan(float u) const
{
	int n = (int)m_Knots.size() - m_Degree - 2;
	if (u == m_Knots[n + 1]) return n;
	int low = m_Degree, high = n + 1;
	int mid = (low + high) / 2;
	while (u < m_Knots[mid] || u >= m_Knots[mid + 1]) { 
		if (u < m_Knots[mid]) high = mid;
		else low = mid;
		mid = (low + high) / 2;
	}
	return mid;
}


glm::vec3 NURBS::Sample(float t) const
{
	float u = t * (m_Knots.back() - m_Knots[0]);
	int knotSpan = KnotSpan(u);
	std::vector<float> basisFuncs = BasisFuncs(u);
	glm::vec4 res = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (unsigned i = 0; i <= m_Degree; i++) {
		res += basisFuncs[i] * m_Points[knotSpan - m_Degree + i];
	}
	return glm::vec3{ res.x, res.y, res.z } / res.w;
}

void NURBS::UpdateKnotVector()
{
	m_Knots.clear();
	for (unsigned i = 0; i <= m_Degree; i++) m_Knots.push_back(0.0f);
	for (float i = 1.0f; i < m_Points.size() - m_Degree; i++)  m_Knots.push_back(i);
	for (unsigned i = 0; i <= m_Degree; i++) m_Knots.push_back((float)m_Points.size() - m_Degree);
}

void NURBS::UpdateSamples()
{
	m_Samples.clear();
	m_Indecies.clear();
	int sampleCount = SAMPLES_PER_EDGE * ((int)m_Points.size() - 1);
	for (int i = 0; i <= sampleCount; i++) {
		m_Samples.push_back(Sample((float)i / sampleCount));
		m_Indecies.push_back(i);
		m_Indecies.push_back(i + 1);
	}
	m_Indecies.pop_back();
	m_Indecies.pop_back();
}

void NURBS::UpdateVertexArray()
{
	m_VertexArray = std::make_unique<VertexArrayBasicLines>(m_Samples, m_Color, m_ID, m_Indecies, 2.0f);
}

glm::vec3 NURBS::Intersect(Ray r, uint32_t subID) const
{
	//FIX
	return { 0.0,0.0,0.0 };
}
