#pragma once

#include "Frustum.h"

#include "glm/gtx/intersect.hpp"

Frustum::Frustum(const Ray& tl, const Ray& tr, const Ray& bl, const Ray& br)
	: m_Origin(tl.GetOrigin())
{
	m_N1 = glm::cross(tl.GetDirection(), tr.GetDirection());
	m_N2 = glm::cross(tr.GetDirection(), br.GetDirection());
	m_N3 = glm::cross(br.GetDirection(), bl.GetDirection());
	m_N4 = glm::cross(bl.GetDirection(), tl.GetDirection());
}

bool Frustum::PartiallyContainsLine(const glm::vec3& a, const glm::vec3& b) const
{
	// TODO: what if intersection fails????????
	//		- read glm source to see what happens or implement my own intersection utils
	glm::vec3 delta = b - a;
	glm::vec3 dir = glm::normalize(delta);
	float size = glm::length(delta);
	float t1, t2;
	glm::intersectRayPlane(a, dir, m_Origin, m_N1, t1);
	glm::intersectRayPlane(a, dir, m_Origin, m_N3, t2);
	if (t1 > t2) std::swap(t1, t2);
	if (std::max(t1, 0.0f) > std::min(t2, size)) return false;
	glm::intersectRayPlane(a, dir, m_Origin, m_N2, t1);
	glm::intersectRayPlane(a, dir, m_Origin, m_N4, t2);
	if (t1 > t2) std::swap(t1, t2);
	return std::max(t1, 0.0f) < std::min(t2, size);
}

bool Frustum::FullyContainsLine(const glm::vec3& a, const glm::vec3& b) const
{
	return Contains(a) && Contains(b);
}

bool Frustum::Contains(const glm::vec3& p) const
{
	glm::vec3 v = p - m_Origin;
	return
		glm::dot(m_N1, v) > 0.0f &&
		glm::dot(m_N2, v) > 0.0f &&
		glm::dot(m_N3, v) > 0.0f &&
		glm::dot(m_N4, v) > 0.0f;
}
