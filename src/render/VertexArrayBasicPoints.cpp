#pragma once

#include "VertexArrayBasicPoints.h"
#include "OpenGLUtils.h"

VertexArrayBasicPoints::VertexArrayBasicPoints(std::vector<glm::vec3> points, std::vector<glm::vec3> colors)
{

}

VertexArrayBasicPoints::~VertexArrayBasicPoints()
{
	GLCall(glDeleteBuffers(1, &m_ID));
}

unsigned VertexArrayBasicPoints::GetIndexCount() const
{
	return 0;
}

void VertexArrayBasicPoints::Render(unsigned id, bool selectable, bool subSelectable, bool selected) const
{

}
