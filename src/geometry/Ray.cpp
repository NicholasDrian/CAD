#pragma once

#include "Ray.h"
#include "glm/gtx/intersect.hpp"

Ray::Ray(glm::vec3 origin, glm::vec3 direction)
	:m_Origin(origin), m_Direction(glm::normalize(direction)) 
{

}

glm::vec3 Ray::At(float t) const {
	return m_Origin + m_Direction * t;
}

bool Ray::IntersectPlane(glm::vec3 origin, glm::vec3 normal, glm::vec3& outPoint) const 
{
	float t;
	bool intersected = glm::intersectRayPlane(m_Origin, m_Direction, origin, normal, t);
	if (intersected) outPoint = At(t);
	return intersected;
}

glm::vec3 Ray::IntersectPlaneUnsafe(glm::vec3 origin, glm::vec3 normal) const
{
	float t;
	bool intersected = glm::intersectRayPlane(m_Origin, m_Direction, origin, normal, t);
	return At(t);
}

void Ray::Print() const {
	std::cout << "ray origin: " << m_Origin.x << ',' << m_Origin.y << ',' << m_Origin.z <<
		" direction: " << m_Direction.x << ',' << m_Direction.y << ',' << m_Direction.z << '\n';
}

bool Ray::IntersectTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, glm::vec3& outPoint) const
{
	glm::vec3 normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));
	return IntersectPlane(p1, normal, outPoint);
}

glm::vec3 Ray::IntersectTriangleUnsafe(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) const
{
	glm::vec3 normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));
	return IntersectPlaneUnsafe(p1, normal);
}


glm::vec3 Ray::ClosestPointOnLine(const glm::vec3& startP, const glm::vec3& endP) const
{
	glm::vec3 a = m_Direction;
	glm::vec3 b = glm::normalize(endP - startP);
	glm::vec3 B = startP;
	if (a == b) return B;

	glm::vec3 A = m_Origin;
	glm::vec3 c = B - A;

	float ab = glm::dot(a, b);
	float ac = glm::dot(a, c);
	float bc = glm::dot(b, c);
	float aa = glm::dot(a, a);
	float bb = glm::dot(b, b);

	return B + b * ((ab * ac - bc * aa) / (aa * bb - ab * ab));
}
