#pragma once

#include "VertexBuffer.h"

#include "OpenGLUtils.h"

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color, GLint id)
	: m_Position(position), m_Normal(normal), m_Color(color), m_ID(id) {}

VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices) : m_VertexCount(vertices.size()), m_ID(0) {

	GLCall(glGenBuffers(1, &m_ID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW));

}

void VertexBuffer::Bind() {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
}

VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1, &m_ID));
}