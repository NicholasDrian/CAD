#pragma once

#include "Intersection.h"

#include <iostream>

bool IntersectRayPlane(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 planeOrigin, glm::vec3 planeNormal, float& outTime, bool allowNegativeT)
{
    float denominator = glm::dot(planeNormal, rayDirection);
    if (denominator == 0.0f) return false;
    outTime = glm::dot(planeOrigin - rayOrigin, planeNormal) / denominator;
    return allowNegativeT || outTime >= 0.0f;
}
