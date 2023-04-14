#pragma once

#include "../render/VertexArrayBasicLines.h"
#include "../geometry/Renderable.h"

#include <vector>
#include <memory>

class ConstructionPlane : public Renderable {

public:

	ConstructionPlane(int majorCount, int minorCount, int spacing);

	void Render() const override;

	inline virtual unsigned GetID() const override { return 0U; }

	virtual AxisAlignedBoundingBox GetBoundingBox() const override;
	virtual AxisAlignedBoundingBox GetSubSelectionBoundingBox() const override;

private:

	std::unique_ptr<VertexArrayBasicLines> m_MajorLinesVertexArray;
	std::unique_ptr<VertexArrayBasicLines> m_MinorLinesVertexArray;

};