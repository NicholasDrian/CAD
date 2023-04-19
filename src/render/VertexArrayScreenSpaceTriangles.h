#pragma once

#include "OpenGLUtils.h"

#include "glm/glm.hpp"

#include <vector>

class VertexArrayScreenSpaceTriangles {

public:

	VertexArrayScreenSpaceTriangles(const std::vector<glm::vec3>& positions, const std::vector<uint32_t>& indices, const glm::vec4& color);
	~VertexArrayScreenSpaceTriangles();

	void Render() const;



private:

	glm::vec4 m_Color;
	unsigned m_IndexCount;
	GLuint m_ID, m_VertexBufferID, m_IndexBufferID;

};