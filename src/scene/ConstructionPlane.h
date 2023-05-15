#pragma once

#include "../render/VertexArrayLines.h"
#include "../geometry/Renderable.h"

#include <vector>
#include <memory>

class ConstructionPlane : public Renderable {

public:

	ConstructionPlane(int majorCount, int minorCount, int spacing);

	void Render() const override;

	inline virtual unsigned GetID() const override { return 0U; }

	// [WARNING] fails if there is no intersection!!!!
	virtual glm::vec3 Intersect(Ray r, uint32_t subID) const override;
	virtual bool IntersectsLocalSpace(Ray r, uint32_t subID, float MaxDistancePixels) const override;


	virtual AxisAlignedBoundingBox GetBoundingBox() const override;
	virtual AxisAlignedBoundingBox GetBoundingBoxLocalSpace(uint32_t subID) const override;
	virtual AxisAlignedBoundingBox GetSubSelectionBoundingBox() const override;

	inline virtual bool IsSelected() const override { return false; }
	inline virtual void Select() override {}
	inline virtual void UnSelect() override {}

	inline virtual const glm::mat4& GetModel() const override { return glm::mat4(1.0f); }


	virtual void BakeSelectionTransform(const glm::mat4& t) override;

private:


	std::unique_ptr<VertexArrayLines> m_MajorLinesVertexArray;
	std::unique_ptr<VertexArrayLines> m_MinorLinesVertexArray;

};