#pragma once

#include "VertexArray.h"

#include <iostream>

#include "../utils/OpenGLUtils.h"

VertexArray::VertexArray(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices, PrimitiveType type) 
	: m_VertCount(vertices.size()), m_IndexCount(indices.size()), m_PrimitiveType(type), m_ID(0)
{
	GLCall(glGenVertexArrays(1, &m_ID));
	Bind();

	GLuint vertexBuffer;
	GLCall(glGenBuffers(1, &vertexBuffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW));
	if (type != PrimitiveType::Points) {
		GLuint indexBuffer;
		GLCall(glGenBuffers(1, &indexBuffer));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices.size(), indices.data(), GL_STATIC_DRAW));
	}

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
	//need to delete buffers still
	GLCall(glDeleteVertexArrays(1, &m_ID));
}

void VertexArray::Bind() const {
	GLCall(glBindVertexArray(m_ID));
}