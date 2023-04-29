#pragma once

#include "../render/OpenglUtils.h"

#include "glm/vec3.hpp"
#include "glm/matrix.hpp"
#include <vector>

struct LineVertex {
	glm::vec3 pos;
	glm::vec3 tan;
};

class VertexArrayLines {

public:

	VertexArrayLines(const std::vector<glm::vec3>& positions, const glm::vec4& color, uint32_t id, const std::vector<unsigned>& indecies, float lineWidth, bool dashed = false);

	virtual ~VertexArrayLines();

	void Render(const glm::mat4& t, unsigned id, bool selectable, bool selected) const;

	void UpdateVertexPositions(const std::vector<glm::vec3>& positions);

protected:

	GLuint m_RenderID, m_VertexBufferID, m_IndexBufferID;
	unsigned m_IndexCount;
	glm::vec4 m_Color;
	GLfloat m_LineWidth;
	bool m_Dashed;

};