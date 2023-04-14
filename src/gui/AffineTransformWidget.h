#pragma once

#include "glm/glm.hpp"
#include "../geometry/BoundingBox.h"

#include <vector>

class AffineTransformWidget {

public:

	AffineTransformWidget(const AxisAlignedBoundingBox& bb);

	void Draw();

	inline glm::mat4 GetDelta();

private:

	glm::mat4 m_Transform, m_InitialTransform;

};