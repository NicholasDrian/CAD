#pragma once

#include "VertexArray.h"
#include "../render/OpenglUtils.h"

#include "glm/vec3.hpp"
#include "glm/matrix.hpp"
#include <vector>

struct ColoredLineVertex {
	glm::vec3 pos;
};

class VertexArrayBasicLines : public VertexArray {

public:

	VertexArrayBasicLines(const std::vector<glm::vec3>& positions, const glm::vec3& color, uint32_t id, const std::vector<unsigned>& indecies, float lineWidth);

	~VertexArrayBasicLines();

	virtual void Render(unsigned id, bool selectable, bool selected) const override;
	virtual PrimitiveType GetPrimitiveType() const override { return PrimitiveType::Line; }
	virtual unsigned GetIndexCount() const override { return m_IndexCount; };

private:

	GLfloat m_LineWidth;
	GLuint m_RenderID;
	GLuint m_VertexBufferID;
	GLuint m_IndexBufferID;
	unsigned m_IndexCount;
	glm::mat4 m_Model;
	glm::vec3 m_Color;

};