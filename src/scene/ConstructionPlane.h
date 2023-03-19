#pragma once

#include <vector>

class ConstructionPlane {

public:

	ConstructionPlane(int majorSpacing, int majorCount, int minorSpacing, int minorCount);

	void Render();

private:

	int m_MajorSpacing, m_MinorSpacing;
	int m_MajorCount, m_MinorCount;

	std::vector<float> m_MajorLines;
	std::vector<float> m_MinorLines;

};