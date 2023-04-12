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
	float epsilon = 0.1f;
	glm::vec3 color(0.4, 0.4, 0.4);
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
	m_MajorLinesVertexArray = std::make_unique<VertexArrayBasicLines>(majorLines, color, GetID(), majorIndices, MAJOR_WIDTH);
	m_MinorLinesVertexArray = std::make_unique<VertexArrayBasicLines>(minorLines, color, GetID(), minorIndices, MINOR_WIDTH);

}

void ConstructionPlane::Render() const {
	m_MajorLinesVertexArray->Render(0, false, false, false);
	m_MinorLinesVertexArray->Render(0, false, false, false);
}