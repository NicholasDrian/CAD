#pragma once

#include "ConstructionPlane.h"
#include "../render/Renderer.h"

#include <stdexcept>

const float MAJOR_WIDTH = 1.5f;
const float MINOR_WIDTH = 1.0f;


ConstructionPlane::ConstructionPlane(int majorCount, int minorCount, int spacing)
{ 
#ifdef CAD_DEBUG
	if (majorCount & 1) throw std::runtime_error("majorCount must be even");
#endif
	std::vector<glm::vec3> majorLines, minorLines;
	std::vector<unsigned> majorIndices, minorIndices;
	int majorIndex = 0, minorIndex = 0;
	float half = (float)majorCount * minorCount * spacing / 2.0f;
	float epsilon = 0.01f;
	glm::vec4 color(0.4, 0.4, 0.4, 1.0);
	for (int i = 0; i <= minorCount * majorCount; i++) {
		if (i == majorCount * minorCount / 2.0) 
		{
			majorLines.emplace_back(-half + i * spacing, -half, -epsilon);
			majorLines.emplace_back(-half + i * spacing, 0, -epsilon);
			majorIndices.push_back(majorIndex++);
			majorIndices.push_back(majorIndex++);

			majorLines.emplace_back(-half, -half + i * spacing, -epsilon);
			majorLines.emplace_back(0, -half + i * spacing, -epsilon);
			majorIndices.push_back(majorIndex++);
			majorIndices.push_back(majorIndex++);

			majorLines.emplace_back(-half + i * spacing, 0, -epsilon);
			majorLines.emplace_back(-half + i * spacing, half, -epsilon);
			majorIndices.push_back(majorIndex++);
			majorIndices.push_back(majorIndex++);

			majorLines.emplace_back(0, -half + i * spacing, -epsilon);
			majorLines.emplace_back(half, -half + i * spacing, -epsilon);\
			majorIndices.push_back(majorIndex++);
			majorIndices.push_back(majorIndex++);
		}
		else if (i % minorCount == 0) 
		{
			majorLines.emplace_back(-half + i * spacing, -half, -epsilon);
			majorLines.emplace_back(-half + i * spacing, half, -epsilon);
			majorIndices.push_back(majorIndex++);
			majorIndices.push_back(majorIndex++);

			majorLines.emplace_back(-half, -half + i * spacing, -epsilon);
			majorLines.emplace_back(half, -half + i * spacing, -epsilon);
			majorIndices.push_back(majorIndex++);
			majorIndices.push_back(majorIndex++);
		}
		else 
		{
			minorLines.emplace_back(-half + i * spacing, -half, -epsilon);
			minorLines.emplace_back(-half + i * spacing, half, -epsilon);
			minorIndices.push_back(minorIndex++);
			minorIndices.push_back(minorIndex++);

			minorLines.emplace_back(-half, -half + i * spacing, -epsilon);
			minorLines.emplace_back(half, -half + i * spacing, -epsilon);
			minorIndices.push_back(minorIndex++);
			minorIndices.push_back(minorIndex++);

		}
	}
	m_MajorLinesVertexArray = std::make_unique<VertexArrayLines>(majorLines, color, GetID(), majorIndices, MAJOR_WIDTH, 0U);
	m_MinorLinesVertexArray = std::make_unique<VertexArrayLines>(minorLines, color, GetID(), minorIndices, MINOR_WIDTH, 0U);

}

glm::vec3 ConstructionPlane::Intersect(Ray r, uint32_t subID) const
{
	glm::vec3 p;
#ifdef CAD_DEBUG
	if (!r.IntersectPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, p))
		throw std::runtime_error("intersection failed");
	return p;
#else
	return r.IntersectPlaneUnsafe({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
#endif
}

bool ConstructionPlane::IntersectsLocalSpace(Ray r, uint32_t subID, float& outT, float MaxDistancePixels) const
{
	float o = r.GetOrigin().z, d = r.GetDirection().z;
	return o * d < 0.0f;
}

void ConstructionPlane::BakeSelectionTransform(const glm::mat4& t)
{
	// noop
}

void ConstructionPlane::Render() const {
	m_MajorLinesVertexArray->Render(glm::mat4(1.0), 0, false, false);
	m_MinorLinesVertexArray->Render(glm::mat4(1.0), 0, false, false);
}

AxisAlignedBoundingBox ConstructionPlane::GetBoundingBox() const
{
	return AxisAlignedBoundingBox();
}

AxisAlignedBoundingBox ConstructionPlane::GetBoundingBoxLocalSpace(uint32_t subID) const
{
	return AxisAlignedBoundingBox();
}

AxisAlignedBoundingBox ConstructionPlane::GetSubSelectionBoundingBox() const
{
	return AxisAlignedBoundingBox();
}