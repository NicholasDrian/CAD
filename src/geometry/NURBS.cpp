#pragma once

#include "NURBS.h"

const unsigned SAMPLES_PER_EDGE = 10;

NURBS::NURBS(std::vector<glm::vec3> points, std::vector<float> weights, unsigned degree, std::vector<float> knots, unsigned id)
	: m_Points(points), m_Weights(weights), m_Degree(degree), m_Knots(knots), m_ID(id)
{
	if (m_Weights.size() == 0) {
		m_Weights = std::vector(points.size(), 1.0f);
	}
	if (knots.size() == 0) {
		for (int i = 0; i <= m_Degree; i++) m_Knots.push_back(0.0f);
		for (float i = 1.0f; i < m_Points.size() - m_Degree; i++)  m_Knots.push_back(i);
		for (int i = 0; i <= m_Degree; i++) m_Knots.push_back((float)m_Points.size() - m_Degree);
	}
	for (float f : m_Knots) std::cout << f << ' '; std::cout << '\n';
	UpdateSamples();
	UpdateVertexArray();
}
	
void NURBS::Render() const
{
	m_VertexArray->Render(m_ID, m_Selectable, m_Selected);
}

void NURBS::UpdateLastPoint(glm::vec3 point)
{
	m_Points.back() = point;
	UpdateSamples(); // shouldnt need to update all samples
	UpdateVertexArray();
}

std::vector<float> NURBS::BasisFuncs(float u) const
{
	int i = KnotSpan(u);
	std::vector<float> res(m_Degree + 1);
	res[0] = 1.0;
	auto left = [&](int j) { return u - m_Knots[i - j + 1]; };
	auto right = [&](int j) { return m_Knots[i + j] - u; };
	for (int j = 1; j <= m_Degree; j++) {
		float saved = 0.0;
		for (int r = 0; r < j; r++) {
			float temp = res[r] / (right(r + 1) + left(j - r));
			res[r] = saved + right(r + 1) * temp;
			saved = left(j - r) * temp;
		}
		res[j] = saved;
	}
	//for (float f : res) std::cout << f << ' '; std::cout << std::endl;
	return res;
}

int NURBS::KnotSpan(float u) const
{
	int n = m_Knots.size() - m_Degree - 2;
	if (u == m_Knots[n + 1]) return n;
	int low = m_Degree, high = n + 1;
	int mid = (low + high) / 2;
	// refactor this disgusting binary search
	while (u < m_Knots[mid] || u >= m_Knots[mid + 1]) { 
		if (u < m_Knots[mid]) high = mid;
		else low = mid;
		mid = (low + high) / 2;
	}
	return mid;
}

// this could very welll be wrong :/
glm::vec3 NURBS::Sample(float t) const
{
	float u = t * (m_Knots.back() - m_Knots[0]);
	std::vector<float> basisFuncs = BasisFuncs(u);
	glm::vec3 res = {0.0,0.0,0.0};
	for (int i = 0; i <= m_Degree; i++) {
		res += basisFuncs[i] * m_Points[KnotSpan(u) - m_Degree + i];
		
		std::cout << res.x << ' ' << res.y << ' ' << res.z << ' ' << std::endl;
	}
	std::cout << '\n';
	return res;
}

void NURBS::UpdateSamples()
{
	m_Samples.clear();
	m_Colors.clear();
	int sampleCount = SAMPLES_PER_EDGE * (m_Points.size() - 1);
	for (int i = 0; i <= sampleCount; i++) {
		m_Samples.push_back(Sample((float)i / sampleCount));
		m_Colors.push_back({ 0.0,0.0,0.0 });
		m_Indecies.push_back(i);
		m_Indecies.push_back(i + 1);
	}
	m_Indecies.pop_back();
	m_Indecies.pop_back();

	for (const auto& p : m_Samples) {
		//std::cout << p.x << ' ' << p.y << ' ' << p.z << std::endl;
	}
}

void NURBS::UpdateVertexArray()
{
	m_VertexArray = std::make_unique<VertexArrayColoredLines>(m_Samples, m_Colors, m_ID, m_Indecies, 2.0f);
}
