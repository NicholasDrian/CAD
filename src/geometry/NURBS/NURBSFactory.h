#pragma once

#include "NURBSurface.h"
#include "NURBS.h"

// Nurbs creating utils
class NURBSFactory {

	NURBSFactory() = delete;

public:

	// modifies input curves...
	static NURBSurface Loft(const std::vector<NURBS*>& curves, int degree = 2);

};