#pragma once

#include "VertexArray.h"

#include "OpenGLUtils.h"
#include "glm/vec3.hpp"
#include "glm/matrix.hpp"
#include <vector>

struct ColoredTriangleVertex {
	glm::vec3 pos, norm;
};

class VertexArrayBasicTriangles : public VertexArray {

public:

	VertexArrayBasicTriangles(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const glm::vec3& color, uint32_t id, const std::vector<unsigned>& indecies);

	~VertexArrayBasicTriangles();

	virtual void Render(unsigned id, bool selectable, bool selected) const override;
	virtual PrimitiveType GetPrimitiveType() const override { return PrimitiveType::Triangle; }
	virtual unsigned GetIndexCount() const override { return m_IndexCount; };

private:

	
	GLuint m_RenderID;
	GLuint m_VertexBufferID;
	GLuint m_IndexBufferID;
	unsigned m_IndexCount;
	glm::mat4 m_Model;
	glm::vec3 m_Color;

};