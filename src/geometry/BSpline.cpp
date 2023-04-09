#pragma once 

#include "BSpline.h"

const unsigned SAMPLES_PER_EDGE = 10U;

BSpline::BSpline(std::vector<glm::vec3> points, unsigned degree, unsigned id)
	: m_Points(points), m_Degree(degree), m_ID(id)
{
	SetSamplePoints();
	UpdateVertexArray();
}

void BSpline::UpdateLastPoint(glm::vec3 point)
{
	m_Points.back() = point;
	SetSamplePoints();
	UpdateVertexArray();
}

void BSpline::Render() const
{
	m_VertexArray->Render(m_ID, m_Selectable, m_Selected);
}

glm::vec3 BSpline::Sample(float t, unsigned degree)
{
	//if (t <= 0) return m_SamplePoints[0];
	//if (t >= m_SamplePoints.size() - 1) return m_SamplePoints.back();
	//if (degree == 0) {
	//	return m_Points[(unsigned)(t + 0.5f)];
	//} else {

	//}
	return glm::vec3();
}

void BSpline::SetSamplePoints()
{
	if (m_Degree == 1) 
	{
		m_SamplePoints = m_Points;
	}
	else 
	{
		m_SamplePoints.clear();
		unsigned degree = std::min(m_Degree, (unsigned)m_Points.size() - 1);
		unsigned samples = SAMPLES_PER_EDGE * (m_Points.size() - 1);
		for (int i = 0; i <= samples; i++) {
			float t = (float)i / samples * (m_SamplePoints.size() - 1);
			m_SamplePoints.push_back(Sample(t, degree));
		}
	}
}

void BSpline::UpdateVertexArray()
{

}
