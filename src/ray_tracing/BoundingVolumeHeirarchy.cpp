#pragma once

#include "BoundingVolumeHeirarchy.h"

#include "../geometry/PolyLine.h"

#include "../debug/Print.h"

//
// TODO : Click with snaps
//		Make every goe have intersectable primitives
//		Make every indices have get primitive bb
//		Bounding Volume Heirarcyy
//		Ray Traced Selection
//		Ray traced Selection + Raster Selection
//


BoundingVolumeHeirarchy::BoundingVolumeHeirarchy(Renderable* r)
	: m_Geometry(r)
{
	int count = r->GetPrimitiveCount();
	std::vector<int> indices(count);
	for (int i = 0; i < count; i++) indices[i] = i;
	m_Root = std::make_unique<BVHNode>(m_Geometry, indices);
}

bool BoundingVolumeHeirarchy::Intersect(const Ray& ray, float& outT) const
{
	glm::mat4 t = glm::inverse(m_Geometry->GetModel());
	Ray r(t * glm::vec4(ray.GetOrigin(), 1.0), t * glm::vec4(ray.GetDirection(), 0.0));
	bool res = m_Root->Intersect(r, outT);
	if (res) outT *= glm::length(t * glm::vec4(ray.GetDirection(), 0.0f));
	return res;
}

bool BoundingVolumeHeirarchy::Intersect(const Ray& ray) const
{
	//todo
	throw std::runtime_error("errrrrorrrrrrr!!!!");
	return false;
}

BVHNode::BVHNode(Renderable* geometry, const std::vector<int>& indices, int depth)
	: m_Geometry(geometry)
{
	//for (int i = 0; i < depth; i++) std::cout << '\t'; std::cout << "brooooo\n";
	std::vector<AxisAlignedBoundingBox> bbs(indices.size());
	for (int i = 0; i < indices.size(); i++) {
		bbs[i] = m_Geometry->GetBoundingBoxLocalSpace(indices[i]);
		m_BoundingBox += bbs[i];
	}
	//for (int i = 0; i < depth; i++) std::cout << "  "; print(m_BoundingBox);
	if (indices.size() <= MAX_PRIMITIVE_PER_NODE) {
		m_Indices = indices;
		return;
	}
	glm::vec3 center = m_BoundingBox.GetCenter();
	int axis = depth % 3;
	std::vector<int> v1, v2;
	//todo use quick select median, not mean! ...silly
	switch (axis) {
	case 0:
		for (int i = 0; i < bbs.size(); i++) {
			glm::vec3 delta = center - bbs[i].GetCenter();
			if (delta.x > 0.0f) v1.push_back(indices[i]);
			else v2.push_back(indices[i]);
		}
		break;
	case 1:
		for (int i = 0; i < bbs.size(); i++) {
			glm::vec3 delta = center - bbs[i].GetCenter();
			if (delta.y > 0.0f) v1.push_back(indices[i]);
			else v2.push_back(indices[i]);
		}
		break;
	default:
		for (int i = 0; i < bbs.size(); i++) {
			glm::vec3 delta = center - bbs[i].GetCenter();
			if (delta.z > 0.0f) v1.push_back(indices[i]);
			else v2.push_back(indices[i]);
		}
	}
	m_ChildA = std::make_unique<BVHNode>(m_Geometry, v1, depth + 1);
	m_ChildB = std::make_unique<BVHNode>(m_Geometry, v2, depth + 1);
}
static int debug = 0;

bool BVHNode::Intersect(const Ray& ray, float& outT) const
{
	//for (int i = 0; i < debug; i++) std::cout << "   "; std::cout << "isect\n";
	glm::vec3 trash;
	if (ray.IntersectBoundingBox(m_BoundingBox, trash)) {
		if (m_ChildA) {
			float t1, t2;
			debug++;
			bool b1 = m_ChildA->Intersect(ray, t1);
			bool b2 = m_ChildB->Intersect(ray, t2);
			debug--;
			if (b1 && b2) outT = std::min(t1, t2);
			else if (b1) outT = t1;
			else if (b2) outT = t2;
			else return false;
			return true;
		}
		else {
			bool intersected = false;
			outT = std::numeric_limits<float>::infinity();
			for (int i : m_Indices) {
				float t = std::numeric_limits<float>::infinity();
				// todo: set parameters based on snap.
				if (m_Geometry->IntersectsLocalSpace(ray, i, t, 10) && t > 0.0f) {
					intersected = true;
					outT = std::min(t, outT);
				}
			}
			return intersected;
		}
	}
	else {
		return false;
	}
}