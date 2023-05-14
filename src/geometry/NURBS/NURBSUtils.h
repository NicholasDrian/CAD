#pragma once

#include "glm/glm.hpp"

#include <vector>

// helper functions for nurbs geometry
class NURBSUtils {

public:

	// todo move more stuff to here

	NURBSUtils() = delete;

	static int Span(const std::vector<float>& knots, float u, int p);

	static std::vector<float> BasisFuncs(const std::vector<float>& knots, float u, int p);

	static std::vector<float> GenericKnotVector(int pointCount, int degree);

	inline static const unsigned SAMPLES_PER_EDGE = 20;

};