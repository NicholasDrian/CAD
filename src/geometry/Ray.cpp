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

void Ray::Print() const {
	std::cout << "ray origin: " << m_Origin.x << ',' << m_Origin.y << ',' << m_Origin.z <<
		" direction: " << m_Direction.x << ',' << m_Direction.y << ',' << m_Direction.z << '\n';
}
