#pragma once

#include "glm/glm.hpp"
#include <iostream>

class Ray {

public :

	Ray(glm::vec3 origin, glm::vec3 direction);

	glm::vec3 At(float t) const;

	bool IntersectPlane(glm::vec3 origin, glm::vec3 normal, glm::vec3& outPoint) const;

	void Print() const;

private:

	glm::vec3 m_Origin, m_Direction;

};