#pragma once

#include "glm/glm.hpp"
#include "../geometry/BoundingBox.h"

void print(glm::vec3, bool newLine = false);
void print(glm::vec4, bool newLine = false);
void print(float, bool newLine = false);
void print(AxisAlignedBoundingBox bb);
void print(const glm::mat4&);