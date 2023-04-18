#pragma once

#include "glm/glm.hpp"
#include "../geometry/BoundingBox.h"

#include <vector>

class AffineTransformWidget {

public:

	AffineTransformWidget(const AxisAlignedBoundingBox& bb);

	void Draw();

	glm::mat4 GetDelta() const;

private:

	glm::mat4 m_Transform, m_InitialTransform, m_Delta;
	const glm::mat4 m_Identity;

};