#pragma once

#include "VertexArray.h"
#include "../render/OpenglUtils.h"

#include "glm/vec3.hpp"
#include "glm/matrix.hpp"
#include <vector>

struct ColoredLineVertex {
	ColoredLineVertex() = default;
	ColoredLineVertex(glm::vec3 Pos, glm::vec3 Col);
	glm::vec3 pos, col;
};

class VertexArrayColoredLines : public VertexArray {

public:

	VertexArrayColoredLines(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& colors, uint32_t id, const std::vector<unsigned>& indecies, float lineWidth);

	~VertexArrayColoredLines();

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

};