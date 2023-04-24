#pragma once

#include "Ray.h"

class Frustum {

public:

	Frustum(const Ray& tl, const Ray& tr, const Ray& bl, const Ray& br);

	bool Contains(const glm::vec3& p) const;

	bool PartiallyContainsLine(glm::vec3 a, glm::vec3 b) const;
	bool FullyContainsLine(const glm::vec3& a, const glm::vec3& b) const;

private:

	glm::vec3 m_Origin, m_NTop, m_NRight, m_NBottom, m_NLeft;

};