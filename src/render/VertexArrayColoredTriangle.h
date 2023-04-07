#pragma once

#include "VertexArray.h"

#include "OpenGLUtils.h"
#include "glm/vec3.hpp"
#include "glm/matrix.hpp"
#include <vector>

struct ColoredTriangleVertex {
	ColoredTriangleVertex() = default;
	ColoredTriangleVertex(glm::vec3 Pos, glm::vec3 Norm, glm::vec3 Col, uint32_t I);
	glm::vec3 pos, norm, col;
	uint32_t i;
};

class VertexArrayColoredTriangle : public VertexArray {

public:

	VertexArrayColoredTriangle(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<glm::vec3>& colors, uint32_t id, const std::vector<unsigned>& indecies);

	~VertexArrayColoredTriangle();

	virtual void Render(unsigned id, bool selectable, bool selected) const override;
	virtual PrimitiveType GetPrimitiveType() const override { return PrimitiveType::Triangle; }
	virtual unsigned GetIndexCount() const override { return m_IndexCount; };

private:

	
	GLuint m_RenderID;
	GLuint m_VertexBufferID;
	GLuint m_IndexBufferID;
	unsigned m_IndexCount;
	glm::mat4 m_Model;

};