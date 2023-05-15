#pragma once

#include "BoundingVolumeHeirarchy.h"

#include "../geometry/PolyLine.h"

//
// TODO : Click with snaps
//		Make every goe have intersectable primitives
//		Make every geo have get primitive bb
//		Bounding Volume Heirarcyy
//		Ray Traced Selection
//		Ray traced Selection + Raster Selection
//


BoundingVolumeHeirarchy::BoundingVolumeHeirarchy(Renderable* r)
	: m_Geometry(r)
{
}

bool BoundingVolumeHeirarchy::Intersect(const Ray& ray, glm::vec3& outPoint) const
{
	glm::mat4 t = glm::inverse(m_Geometry->GetModel());
	Ray r(t * glm::vec4(ray.GetOrigin(), 1.0), t * glm::vec4(ray.GetDirection(), 0.0));
	return m_Root->Intersect(r, outPoint);
}

BVHNode::BVHNode(BoundingVolumeHeirarchy* heirarchy, const std::vector<int>& geo)
	: m_Heirarchy(heirarchy)
{
	//todo
}

bool BVHNode::Intersect(const Ray& ray, glm::vec3& outPoint) const
{
	//todo
	return false;
}
