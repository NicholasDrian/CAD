#pragma once

#include "Ray.h"

Ray::Ray(glm::vec3 origin, glm::vec3 direction)
	:m_Origin(origin), m_Direction(glm::normalize(direction)) 
{

}

bool Ray::IntersectPlane(glm::vec3 origin, glm::vec3 normal, glm::vec3& outPoint) const 
{
	float denominator = glm::dot(m_Direction, normal);
	if (denominator == 0) return false;
	float t = glm::dot(origin - m_Origin, normal) / denominator;
	if (t < 0) return false;
	outPoint = m_Origin + m_Direction * t;
	return true;
}