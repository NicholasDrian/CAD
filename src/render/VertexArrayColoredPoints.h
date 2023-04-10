#pragma once

#include "VertexArray.h"

#include "glm/glm.hpp"
#include "GL/glew.h"

#include <vector>


//todo!
class VertexArrayColoredPoints : public VertexArray {

public:

	VertexArrayColoredPoints(std::vector<glm::vec3> points, std::vector<glm::vec3> colors);
	~VertexArrayColoredPoints();

	virtual PrimitiveType GetPrimitiveType() const override { return PrimitiveType::Point; };
	virtual unsigned GetIndexCount() const override;
	virtual void Render(unsigned id, bool selectable, bool selected) const override;

private:


	GLuint m_ID;

};