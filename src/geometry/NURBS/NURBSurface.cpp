#pragma once

#include "NURBSurface.h"
#include "NURBSUtils.h"
#include "../../scene/Scene.h"

NURBSurface::NURBSurface(const std::vector<std::vector<glm::vec4>>& points, const std::vector<float>& knotsU, const std::vector<float>& knotsV, int degreeU, int degreeV)
	: m_Points(points), m_KnotsU(knotsU), m_KnotsV(knotsV), m_DegreeU(degreeU), m_DegreeV(degreeV), m_ID(Scene::GetNewID()), m_Model(1.0)
{
	ControlPointsUpdated();
}

void NURBSurface::Render() const
{
	m_VertexArray->Render(m_Model, m_ID, m_Selectable, m_Selected);
}

AxisAlignedBoundingBox NURBSurface::GetBoundingBox() const
{
	// loose bb because its way cheaper...
	//todo
	return AxisAlignedBoundingBox();
}

AxisAlignedBoundingBox NURBSurface::GetSubSelectionBoundingBox() const
{
	//todo
	return AxisAlignedBoundingBox();
}

void NURBSurface::BakeSelectionTransform(const glm::mat4& t)
{
	//todo
}

glm::vec3 NURBSurface::Intersect(Ray r, uint32_t subID) const
{
	//todo
	return glm::vec3();
}

void NURBSurface::ControlPointsUpdated()
{
	int sampleCountU = NURBSUtils::SAMPLES_PER_EDGE * (m_Points.size() - 1);
	int sampleCountV = NURBSUtils::SAMPLES_PER_EDGE * (m_Points[0].size() - 1);
	float firstKnotU = m_KnotsU[0];
	float firstKnotV = m_KnotsV[0];
	float knotSizeU = m_KnotsU.back() - firstKnotU;
	float knotSizeV = m_KnotsV.back() - firstKnotV;
	float stepU = knotSizeU / sampleCountU;
	float stepV = knotSizeV / sampleCountV;
	m_Samples.resize(sampleCountU + 1);
	for (auto& v : m_Samples) v.resize(sampleCountV + 1);
	for (int i = 0; i <= sampleCountU; i++) {
		for (int j = 0; j <= sampleCountV; j++) {
			m_Samples[i][j] = Sample(firstKnotU + i * stepU, firstKnotV + j * stepV);
		}
	}
	// create vertex array...
}

glm::vec3 NURBSurface::Sample(float u, float v) const
{
	int USpan = NURBSUtils::Span(m_KnotsU, u, m_DegreeU);
	int VSpan = NURBSUtils::Span(m_KnotsV, v, m_DegreeV);
	std::vector<float> basisFuncsU = NURBSUtils::BasisFuncs(m_KnotsU, u, m_DegreeU);
	std::vector<float> basisFuncsV = NURBSUtils::BasisFuncs(m_KnotsV, v, m_DegreeV);
	glm::vec4 intermediate;
	glm::vec4 res = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (int i = 0; i <= m_DegreeU; i++) {
		intermediate = { 0.0f, 0.0f, 0.0f, 0.0f };
		for (int j = 0; j <= m_DegreeV; j++) {
			intermediate += m_Points[USpan - m_DegreeU + i][VSpan - m_DegreeV + j] * basisFuncsV[j];
		}
		res += basisFuncsU[i] * intermediate;
	}
	return glm::vec3{ res.x / res.w, res.y / res.w, res.z / res.w };
}
