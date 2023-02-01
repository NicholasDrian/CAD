#pragma once

#include "IndexBuffer.h"

#include "../utils/OpenGLUtils.h"

IndexBuffer::IndexBuffer(const std::vector<unsigned>& indices) : m_IndexCount(indices.size()), m_ID(0) {

	GLCall(glGenBuffers(1, &m_ID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices.size(), indices.data(), GL_STATIC_DRAW));

}

IndexBuffer::~IndexBuffer() {
	GLCall(glDeleteBuffers(1, &m_ID));
}

void IndexBuffer::Bind() {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
}