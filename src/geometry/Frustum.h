#pragma once

#include "Ray.h"

class Frustum {

public:

	Frustum(const Ray& tl, const Ray& tr, const Ray& bl, const Ray& br);

	bool Contains(const glm::vec3& p) const;

	bool PartiallyContainsLine(const glm::vec3& a, const glm::vec3& b) const;
	bool FullyContainsLine(const glm::vec3& a, const glm::vec3& b) const;

private:

	glm::vec3 m_Origin, m_N1, m_N2, m_N3, m_N4;

};