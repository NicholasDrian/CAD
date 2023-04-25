#pragma once

#include "Frustum.h"

#include "../debug/Print.h"

#include "../scene/Scene.h"

Frustum::Frustum(const Ray& tl, const Ray& tr, const Ray& bl, const Ray& br)
	: m_Origin(tl.GetOrigin())
{
	m_NTop = glm::normalize(glm::cross(tl.GetDirection(), tr.GetDirection()));
	m_NRight = glm::normalize(glm::cross(tr.GetDirection(), br.GetDirection()));
	m_NBottom = glm::normalize(glm::cross(br.GetDirection(), bl.GetDirection()));
	m_NLeft = glm::normalize(glm::cross(bl.GetDirection(), tl.GetDirection()));
}

bool Frustum::PartiallyContainsLine(glm::vec3 a, glm::vec3 b) const
{
	// TODO: move this to compute shader
	// WARNING: unhandled parallel edge cases, would slow down very hot path dramatically. 
	//			these edge cases are very rare and are being ignored for now. Once this is
	//			moved to compute shader, should probably handle all edge cases. 

	glm::vec3 dir = b - a;

	const float size = glm::length(dir);
	dir /= size;	

	Ray r(a, dir);

	float tTop, tRight, tBottom, tLeft;
	bool p1 = r.IntersectPlane(m_Origin, m_NTop, tTop, true);
	bool p2 = r.IntersectPlane(m_Origin, m_NRight, tRight, true);
	bool p3 = r.IntersectPlane(m_Origin, m_NBottom, tBottom, true);
	bool p4 = r.IntersectPlane(m_Origin, m_NLeft, tLeft, true);

	float near = 0.0f, far = size;
	
	if (glm::dot(m_NTop, dir) < 0.0f)		far = std::min(far, tTop);		else near = std::max(near, tTop);
	if (glm::dot(m_NRight, dir) < 0.0f)		far = std::min(far, tRight);	else near = std::max(near, tRight);
	if (glm::dot(m_NBottom, dir) < 0.0f)	far = std::min(far, tBottom);	else near = std::max(near, tBottom);
	if (glm::dot(m_NLeft, dir) < 0.0f)		far = std::min(far, tLeft);		else near = std::max(near, tLeft);

	return near <= far;
}

bool Frustum::FullyContainsLine(const glm::vec3& a, const glm::vec3& b) const
{
	return Contains(a) && Contains(b);
}

bool Frustum::Contains(const glm::vec3& p) const
{
	glm::vec3 v = p - m_Origin;
	return
		glm::dot(m_NTop, v) > 0.0f &&
		glm::dot(m_NRight, v) > 0.0f &&
		glm::dot(m_NBottom, v) > 0.0f &&
		glm::dot(m_NLeft, v) > 0.0f;
}
