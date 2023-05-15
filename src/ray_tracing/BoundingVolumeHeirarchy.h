#pragma once

#include "../geometry/Ray.h"
#include "../geometry/Renderable.h"

#include "glm/glm.hpp"

#include <memory>
#include <vector>


class BoundingVolumeHeirarchy;

struct BVHNode {

	BVHNode(BoundingVolumeHeirarchy* heirarchy, const std::vector<int>& geo);

	bool Intersect(const Ray& ray, glm::vec3& outPoint) const;

	inline static constexpr int MAX_PRIMITIVE_PER_NODE = 5;

	AxisAlignedBoundingBox m_BoundingBox;
	BoundingVolumeHeirarchy* m_Heirarchy;
	std::vector<unsigned> m_Geometry;
	std::unique_ptr<BVHNode> m_ChildA, m_ChildB;

};

class BoundingVolumeHeirarchy {

public:

	BoundingVolumeHeirarchy(Renderable* r);

	bool Intersect(const Ray& ray, glm::vec3& outPoint) const;

private:

	Renderable* m_Geometry;
	std::unique_ptr<BVHNode> m_Root;

};


