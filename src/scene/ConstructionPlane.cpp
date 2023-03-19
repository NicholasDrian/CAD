#pragma once

#include "ConstructionPlane.h"
#include "../render/Renderer.h"

const int MajorWidth = 0.1;
const int MinorWidth = 0.05;


ConstructionPlane::ConstructionPlane(int majorSpacing, int majorCount, int minorSpacing, int minorCount) 
	: m_MajorSpacing(majorSpacing), m_MajorCount(majorCount), m_MinorSpacing(minorSpacing), m_MinorCount(minorCount)
{
	float half = (float)m_MajorCount * m_MajorSpacing / 2.0;
	for (int i = 0; i <= m_MinorCount * m_MajorCount; i++) {
		if (i % minorCount == 0) 
		{
			m_MajorLines.push_back(-half);
			m_MajorLines.push_back(-half + i * m_MinorSpacing);
			m_MajorLines.push_back(0.0);
		}
		else 
		{
			m_MinorLines.push_back(-half);
			m_MinorLines.push_back(-half + i * m_MinorSpacing);
			m_MinorLines.push_back(0.0);
		}
	}
}

void ConstructionPlane::Render() {
	//Renderer::Render()
}