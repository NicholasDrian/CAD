#pragma once 

#include "Renderable.h"

#include "glm/glm.hpp"

#include "../render/VertexBuffer.h"
#include "../render/VertexArray.h"
#include "../render/IndexBuffer.h"

class Triangle : public Renderable {

public:

	Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);

	virtual void Render() const override;

private:

	glm::vec3 m_A, m_B, m_C;

	std::unique_ptr<VertexBuffer> m_VBO;
	std::unique_ptr<IndexBuffer> m_IBO;
	std::unique_ptr<VertexArray> m_VAO;

};