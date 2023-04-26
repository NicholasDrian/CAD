#pragma once

#include "glm/glm.hpp"
#include <iostream>

class Ray {

public :

	Ray(glm::vec3 origin, glm::vec3 direction);

	glm::vec3 At(float t) const;
	inline glm::vec3 GetOrigin() const { return m_Origin; }
	inline glm::vec3 GetDirection() const { return m_Direction; }

	bool IntersectPlane(glm::vec3 origin, glm::vec3 normal, float& outTime, bool allowNegativeT = false) const;
	bool IntersectPlane(glm::vec3 origin, glm::vec3 normal, glm::vec3& outPoint, bool allowNegativeT = false) const;
	bool IntersectPlane(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float& outTime, bool allowNegativeT = false) const;
	bool IntersectPlane(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, glm::vec3& outPoint, bool allowNegativeT = false) const;

	bool IntersectTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, glm::vec3& outPoint, bool allowNegativeT = false) const;
	bool IntersectTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float& outTime, bool allowNegativeT = false) const;

	// Closest point to line extended to infinte length.
	glm::vec3 ClosestPointOnLine(const glm::vec3& startP, const glm::vec3& endP) const;

private:

	glm::vec3 m_Origin, m_Direction;

};