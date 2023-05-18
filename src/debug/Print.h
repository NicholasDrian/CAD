#pragma once

#include "glm/glm.hpp"
#include "../geometry/BoundingBox.h"
#include "../ray_tracing/BoundingVolumeHeirarchy.h"
#include "../geometry/Ray.h"

void print(glm::vec3, bool newLine = false);
void print(glm::vec4, bool newLine = false);
void print(float, bool newLine = false);
void print(AxisAlignedBoundingBox bb);
void print(BVHNode* node, int depth = 0);
void print(Renderable* r);
void print(Ray r);
void print(const char* s);
void print(const glm::mat4&);