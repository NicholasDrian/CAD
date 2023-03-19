//#pragma once
//
//#include "VertexArray.h"
//
//#include <iostream>
//
//#include "OpenGLUtils.h"

//VertexArray::VertexArray(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveType type, bool selectable)
//	: m_PrimitiveType(type), m_ID(0), m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer), m_Selectable(selectable)
//{
//	GLCall(glGenVertexArrays(1, &m_ID));
//	Bind();
//
//	if (m_IndexBuffer) m_IndexBuffer->Bind();
//	m_VertexBuffer->Bind();
//
//	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Position)));
//	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Normal)));
//	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Color)));
//	GLCall(glVertexAttribIPointer(3, 1, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_ID)));
//
//	GLCall(glEnableVertexAttribArray(0));
//	GLCall(glEnableVertexAttribArray(1));
//	GLCall(glEnableVertexAttribArray(2));
//	GLCall(glEnableVertexAttribArray(3));
//
//}

//VertexArray::~VertexArray() {
//	GLCall(glDeleteVertexArrays(1, &m_ID));
//}
//
//void VertexArray::Bind() const {
//	GLCall(glBindVertexArray(m_ID));
//}