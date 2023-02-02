#pragma once

#include "Triangle.h"

#include "../render/Renderer.h"

Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) 
	: m_A(a), m_B(b), m_C(c)
{
	m_IBO = std::make_unique<IndexBuffer>(std::vector{ 0U, 1U, 2U });
	m_VBO = std::make_unique<VertexBuffer>(std::vector{
		Vertex{ m_A, {0.0, 0.0, 1.0}, {1.0, 0.0, 0.0}, 69},
		Vertex{ m_B, {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, 69},
		Vertex{ m_C, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, 69}
	});
	m_VAO = std::make_unique<VertexArray>(m_VBO.get(), m_IBO.get(), PrimitiveType::Triangles);
}

void Triangle::Render() const 
{
	Renderer::Render(*m_VAO);
}