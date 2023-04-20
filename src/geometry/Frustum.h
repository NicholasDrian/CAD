#pragma once

#include "Ray.h"

class Frustum {

public:

	Frustum(const Ray& tl, const Ray& br);

	bool Contains(glm::vec3 p) const;

private:

	glm::vec3 m_Origin, m_N1, m_N2, m_N3, m_N4;

};