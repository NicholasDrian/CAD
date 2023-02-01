#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"

#include <vector>


struct Vertex {

	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color, GLint id);

	glm::vec3 m_Position, m_Normal, m_Color;
	GLint m_ID;

};

class VertexBuffer {

public:

	VertexBuffer(const std::vector<Vertex>& vertices);
	~VertexBuffer();

	void Bind();

	inline size_t GetVertexCount() const { return m_VertexCount; }

private:

	GLuint m_ID;
	size_t m_VertexCount;

};