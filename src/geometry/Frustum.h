#pragma once

//#include "Ray.h"
#include "glm/glm.hpp"

class Frustum {

public:

	//Frustum(const Ray& tl, const Ray& tr, const Ray& bl, const Ray& br);
	Frustum(const glm::vec3& origin, const glm::vec3& tl, const glm::vec3& tr, const glm::vec3& bl, const glm::vec3& br);

	bool Contains(const glm::vec3& p) const;

	bool PartiallyContainsLine(glm::vec3 a, glm::vec3 b) const;
	bool FullyContainsLine(const glm::vec3& a, const glm::vec3& b) const;

private:

	glm::vec3 m_Origin, m_NTop, m_NRight, m_NBottom, m_NLeft;

};