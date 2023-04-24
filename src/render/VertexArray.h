
#pragma once

#include "glm/glm.hpp"

// could delete this interface...

class VertexArray {

public:

	virtual void Render(const glm::mat4& t, unsigned id, bool selectable, bool subSelectable, bool selected) const = 0;

};