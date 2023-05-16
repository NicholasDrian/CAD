#pragma once

#include "../geometry/Ray.h"
#include "../geometry/Renderable.h"

#include "glm/glm.hpp"

#include <memory>
#include <vector>


class BoundingVolumeHeirarchy;

struct BVHNode {

	BVHNode(Renderable* geometry, const std::vector<int>& indices, int depth = 0);

	bool Intersect(const Ray& ray, float& outT) const;

	inline static constexpr int MAX_PRIMITIVE_PER_NODE = 5;

	AxisAlignedBoundingBox m_BoundingBox;
	Renderable* m_Geometry;
	std::vector<unsigned> m_Indices;
	std::unique_ptr<BVHNode> m_ChildA, m_ChildB;

};

class BoundingVolumeHeirarchy {

public:

	BoundingVolumeHeirarchy(Renderable* r);

	bool Intersect(const Ray& ray, float& outT) const;

	inline Renderable* GetGeometry() { return m_Geometry; }

private:

	Renderable* m_Geometry;
	std::unique_ptr<BVHNode> m_Root;

};


