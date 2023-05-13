#pragma once

#include "NURBS.h"

class Circle : public NURBS {

public:

	Circle(glm::vec3 a, glm::vec3 b, glm::vec3 c);

	Circle(glm::vec3 normal, glm::vec3 center, float radious);

private:

	void Create(glm::vec3 normal, glm::vec3 center, float radious);

};