#pragma once

#include "glm/glm.hpp"

class Ray {

public :

	Ray(glm::vec3 origin, glm::vec3 direction);

	bool IntersectPlane(glm::vec3 origin, glm::vec3 normal, glm::vec3& outPoint) const;

private:

	glm::vec3 m_Origin, m_Direction;

};