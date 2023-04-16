#pragma once


#include "glm/glm.hpp"
#include "BoundingBox.h"
#include "Ray.h"

#include <stdint.h>
#include <vector>

class Renderable {

public:

	virtual void Render() const = 0;
	virtual unsigned GetID() const = 0;
	virtual AxisAlignedBoundingBox GetBoundingBox() const = 0;
	virtual AxisAlignedBoundingBox GetSubSelectionBoundingBox() const = 0;
	virtual void BakeSelectionTransform(const glm::mat4& t) = 0;

	virtual glm::vec3 Intersect(Ray r, uint32_t subID) const = 0;

	virtual void AddSubSelection(uint32_t subID) {};
	virtual void RemoveSubSelection(uint32_t subID) {};
	virtual void ClearSubSelection() {};

	bool m_Selectable = true;
	bool m_Selected = false;

};