#pragma once

#include "NURBSurface.h"
#include "NURBSUtils.h"
#include "../../scene/Scene.h"
#include "../../debug/Print.h"

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
	std::cout << sampleCountU << ' ' << sampleCountV << std::endl;
	float firstKnotU = m_KnotsU[0];
	float firstKnotV = m_KnotsV[0];
	float knotSizeU = m_KnotsU.back() - firstKnotU;
	float knotSizeV = m_KnotsV.back() - firstKnotV;
	float stepU = knotSizeU / sampleCountU;
	float stepV = knotSizeV / sampleCountV;
	m_Samples.resize((sampleCountU + 1) * (sampleCountV + 1));
	for (int i = 0; i <= sampleCountU; i++) {
		for (int j = 0; j <= sampleCountV; j++) {
			m_Samples[i * (sampleCountV + 1) + j] = Sample(firstKnotU + i * stepU, firstKnotV + j * stepV);
			print(m_Samples[i * (sampleCountV + 1) + j]);
		} std::cout << '\n';
	}
	std::vector<unsigned> indices;
	for (int i = 0; i < sampleCountU; i++) {
		for (int j = 0; j < sampleCountV; j++) {
			indices.push_back(i * (sampleCountV + 1) + j);
			indices.push_back(i * (sampleCountV + 1) + j + 1);
			indices.push_back((i + 1) * (sampleCountV + 1) + j);

			indices.push_back(i * (sampleCountV + 1) + j + 1);
			indices.push_back((i + 1) * (sampleCountV + 1) + j + 1);
			indices.push_back((i + 1) * (sampleCountV + 1) + j);
		}
	}
	//todo normals
	glm::vec4 color{ 1.0,0.0,0.0,1.0 };
	m_VertexArray = std::make_unique<VertexArrayTriangles>(m_Samples, m_Samples, color, indices);
}

glm::vec3 NURBSurface::Sample(float u, float v) const
{
	int USpan = NURBSUtils::Span(m_KnotsU, u, m_DegreeU);
	int VSpan = NURBSUtils::Span(m_KnotsV, v, m_DegreeV);
	std::vector<float> basisFuncsU = NURBSUtils::BasisFuncs(m_KnotsU, u, m_DegreeU);
	std::vector<float> basisFuncsV = NURBSUtils::BasisFuncs(m_KnotsV, v, m_DegreeV);
	glm::vec4 res = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (int i = 0; i <= m_DegreeU; i++) {
		for (int j = 0; j <= m_DegreeV; j++) 
			res += m_Points[USpan - m_DegreeU + i][VSpan - m_DegreeV + j] * (basisFuncsV[j] * basisFuncsU[i]);
	}
	return glm::vec3{ res.x / res.w, res.y / res.w, res.z / res.w };
}
