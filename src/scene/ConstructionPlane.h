#pragma once

#include "../render/VertexArrayColoredLines.h"
#include "../geometry/Renderable.h"

#include <vector>
#include <memory>

class ConstructionPlane : public Renderable {

public:

	ConstructionPlane(int majorCount, int minorCount, int spacing);

	void Render() const override;

private:

	std::unique_ptr<VertexArrayColoredLines> m_MajorLinesVertexArray;
	std::unique_ptr<VertexArrayColoredLines> m_MinorLinesVertexArray;

};