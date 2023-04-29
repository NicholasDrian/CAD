#pragma once

#include "Renderable.h"

#include <memory>

//
// Strategy:
// - fix mesh
// - add parent
// - add degree elevation and knot refinement to nurbs curvs
// - only then can we have the long awaited NURBS surface...
//

class NURBSurface : public Renderable {

public:

	NURBSurface(
		const std::vector<std::vector<glm::vec3>>& points, 
		const std::vector<std::vector<float>>& weights, 
		const std::vector<std::vector<float>>& knots,
		int p, int q);

	virtual void Render() const override;

	inline virtual unsigned GetID() const override { return m_ID; }

	virtual AxisAlignedBoundingBox GetBoundingBox() const override;
	virtual AxisAlignedBoundingBox GetSubSelectionBoundingBox() const override;

	virtual void BakeSelectionTransform(const glm::mat4& t) override;

	virtual glm::vec3 Intersect(Ray r, uint32_t subID) const override;

	inline virtual bool IsSelected() const override { return m_Selected; }
	inline virtual void Select() override { m_Selected = true; }
	inline virtual void UnSelect() override { m_Selected = false; }

private:

	bool m_Selected;
	unsigned m_ID;

};