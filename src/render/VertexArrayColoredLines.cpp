#pragma once 

#include "VertexArrayColoredLines.h"
#include "shaders/ShaderManager.h"


ColoredLineVertex::ColoredLineVertex(glm::vec3 Pos, glm::vec3 Col)
	: pos(Pos), col(Col) { }


VertexArrayColoredLines::VertexArrayColoredLines(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& colors, uint32_t id, const std::vector<unsigned>& indices, float lineWidth)
	: m_IndexCount(indices.size()), m_LineWidth(lineWidth), m_Model(glm::mat4(1.0))
{

	std::vector<ColoredLineVertex> data;
	for (int i = 0; i < positions.size(); i++) {
		data.emplace_back(positions[i], colors[i]);
		//  TODO: id is a waste of space, should move to uniform and set durring bind
	}

	GLCall(glGenBuffers(1, &m_VertexBufferID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(ColoredLineVertex) * data.size(), data.data(), GL_STATIC_DRAW));

	GLCall(glGenBuffers(1, &m_IndexBufferID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), indices.data(), GL_STATIC_DRAW));

	GLCall(glGenVertexArrays(1, &m_RenderID));
	GLCall(glBindVertexArray(m_RenderID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(2));

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredLineVertex),
		(void*)offsetof(ColoredLineVertex, pos)));
	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredLineVertex),
		(void*)offsetof(ColoredLineVertex, col)));

}

VertexArrayColoredLines::~VertexArrayColoredLines()
{
	GLCall(glDeleteBuffers(1, &m_IndexBufferID));
	GLCall(glDeleteBuffers(1, &m_VertexBufferID));
	GLCall(glDeleteVertexArrays(1, &m_RenderID));
}

void VertexArrayColoredLines::Render(unsigned id, bool selectable, bool selected) const {
	ShaderManager::Bind(ShaderProgramType::ColoredLineShader);
	ShaderManager::UpdateLocalUniforms(m_Model, selectable, selected, id);
	GLCall(glBindVertexArray(m_RenderID));
	GLCall(glLineWidth(m_LineWidth));
	GLCall(glDrawElements(GL_LINES, GetIndexCount(), GL_UNSIGNED_INT, (GLvoid*)0));
}