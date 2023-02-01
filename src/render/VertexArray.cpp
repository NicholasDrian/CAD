#pragma once

#include "VertexArray.h"

#include <iostream>

#include "../utils/OpenGLUtils.h"

VertexArray::VertexArray(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, PrimitiveType type) 
	: m_PrimitiveType(type), m_ID(0), m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer)
{
	GLCall(glGenVertexArrays(1, &m_ID));
	Bind();

	m_VertexBuffer->Bind();
	if (m_IndexBuffer) m_IndexBuffer->Bind();

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glEnableVertexAttribArray(3));

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Position)));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Normal)));
	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Color)));
	GLCall(glVertexAttribIPointer(3, 1, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_ID)));

}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_ID));
}

void VertexArray::Bind() const {
	GLCall(glBindVertexArray(m_ID));
}