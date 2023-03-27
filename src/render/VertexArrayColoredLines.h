#pragma once

#include "VertexArray.h"
#include "../render/OpenglUtils.h"

#include "glm/vec3.hpp"
#include "glm/matrix.hpp"
#include <vector>

struct ColoredLineVertex {
	ColoredLineVertex() = default;
	ColoredLineVertex(glm::vec3 Pos, glm::vec3 Col, uint32_t I);
	glm::vec3 pos, col;
	uint32_t i;
};

class VertexArrayColoredLines : public VertexArray {

public:

	VertexArrayColoredLines(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& colors, uint32_t id, const std::vector<unsigned>& indecies, float lineWidth, bool selectable = true);

	~VertexArrayColoredLines();

	virtual void Render() const override;
	virtual PrimitiveType GetPrimitiveType() const override { return PrimitiveType::Line; }
	virtual bool IsSelectable() const override { return m_Selectable; }
	virtual unsigned GetIndexCount() const override { return m_IndexCount; };

private:

	GLfloat m_LineWidth;
	GLuint m_ID;
	GLuint m_VertexBufferID;
	GLuint m_IndexBufferID;
	bool m_Selectable;
	bool m_Selected;
	unsigned m_IndexCount;
	glm::mat4 m_Model;

};