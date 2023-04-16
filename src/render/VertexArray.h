
#pragma once

#include "glm/glm.hpp"

enum class PrimitiveType {
	None = 0,
	Triangle,
	Line,
	Point
};

class VertexArray {

public:

	virtual PrimitiveType GetPrimitiveType() const = 0;
	virtual unsigned GetIndexCount() const = 0;
	virtual void Render(const glm::mat4& t, unsigned id, bool selectable, bool subSelectable, bool selected) const = 0;

};