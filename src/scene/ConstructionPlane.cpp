#pragma once

#include "ConstructionPlane.h"
#include "../render/Renderer.h"

#include <stdexcept>

const int MajorWidth = 0.1;
const int MinorWidth = 0.05;


ConstructionPlane::ConstructionPlane(int majorCount, int minorCount, int spacing)
{ 
#ifdef CAD_DEBUG
	if (majorCount & 1) throw std::runtime_error("majorCount must be even");
#endif
	std::vector<glm::vec3> majorLines, minorLines, majorColors, minorColors;
	std::vector<unsigned> majorIndices, minorIndices;
	int majorIndex = 0, minorIndex = 0;
	float half = (float)majorCount * minorCount * spacing / 2.0;
	float epsilon = 0.1;
	glm::vec3 color(0.4, 0.4, 0.4);
	for (int i = 0; i <= minorCount * majorCount; i++) {
		if (i == majorCount * minorCount / 2.0) 
		{
			majorLines.emplace_back(-half + i * spacing, -half, -epsilon);
			majorLines.emplace_back(-half + i * spacing, 0, -epsilon);
			majorColors.push_back(color);
			majorColors.push_back(color);
			majorIndices.push_back(majorIndex++);
			majorIndices.push_back(majorIndex++);

			majorLines.emplace_back(-half, -half + i * spacing, -epsilon);
			majorLines.emplace_back(0, -half + i * spacing, -epsilon);
			majorColors.push_back(color);
			majorColors.push_back(color);
			majorIndices.push_back(majorIndex++);
			majorIndices.push_back(majorIndex++);

			majorLines.emplace_back(-half + i * spacing, 0, -epsilon);
			majorLines.emplace_back(-half + i * spacing, half, -epsilon);
			majorColors.emplace_back(0.2, 1.0, 0.2);
			majorColors.emplace_back(0.2, 1.0, 0.2);
			majorIndices.push_back(majorIndex++);
			majorIndices.push_back(majorIndex++);

			majorLines.emplace_back(0, -half + i * spacing, -epsilon);
			majorLines.emplace_back(half, -half + i * spacing, -epsilon);
			majorColors.emplace_back(1.0, 0.2, 0.2);
			majorColors.emplace_back(1.0, 0.2, 0.2);
			majorIndices.push_back(majorIndex++);
			majorIndices.push_back(majorIndex++);
		}
		else if (i % minorCount == 0) 
		{
			majorLines.emplace_back(-half + i * spacing, -half, -epsilon);
			majorLines.emplace_back(-half + i * spacing, half, -epsilon);
			majorColors.push_back(color);
			majorColors.push_back(color);
			majorIndices.push_back(majorIndex++);
			majorIndices.push_back(majorIndex++);

			majorLines.emplace_back(-half, -half + i * spacing, -epsilon);
			majorLines.emplace_back(half, -half + i * spacing, -epsilon);
			majorColors.push_back(color);
			majorColors.push_back(color);
			majorIndices.push_back(majorIndex++);
			majorIndices.push_back(majorIndex++);
		}
		else 
		{
			minorLines.emplace_back(-half + i * spacing, -half, -epsilon);
			minorLines.emplace_back(-half + i * spacing, half, -epsilon);
			minorColors.push_back(color);
			minorColors.push_back(color);
			minorIndices.push_back(minorIndex++);
			minorIndices.push_back(minorIndex++);

			minorLines.emplace_back(-half, -half + i * spacing, -epsilon);
			minorLines.emplace_back(half, -half + i * spacing, -epsilon);
			minorColors.push_back(color);
			minorColors.push_back(color);
			minorIndices.push_back(minorIndex++);
			minorIndices.push_back(minorIndex++);

		}
	}
	m_MajorLinesVertexArray = std::make_unique<VertexArrayColoredLines>(majorLines, majorColors, 420, majorIndices, 1.5f, false);
	m_MinorLinesVertexArray = std::make_unique<VertexArrayColoredLines>(minorLines, minorColors, 420, minorIndices, 1.0f, false);

}

void ConstructionPlane::Render() const {
	m_MajorLinesVertexArray->Render();
	m_MinorLinesVertexArray->Render();
}