#pragma once

#include "glm/glm.hpp"

bool IntersectRayPlane(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 planeOrigin, glm::vec3 planeNormal, float& outTime, bool allowNegativeT = false);