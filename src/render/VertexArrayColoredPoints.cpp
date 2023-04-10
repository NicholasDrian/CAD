#pragma once

#include "VertexArrayColoredPoints.h"
#include "OpenGLUtils.h"

VertexArrayColoredPoints::VertexArrayColoredPoints(std::vector<glm::vec3> points, std::vector<glm::vec3> colors)
{

}

VertexArrayColoredPoints::~VertexArrayColoredPoints()
{
	GLCall(glDeleteBuffers(1, &m_ID));
}

unsigned VertexArrayColoredPoints::GetIndexCount() const
{
	return 0;
}

void VertexArrayColoredPoints::Render(unsigned id, bool selectable, bool selected) const
{

}
