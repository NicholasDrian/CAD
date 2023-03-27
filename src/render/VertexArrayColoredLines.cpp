#pragma once 

#include "VertexArrayColoredLines.h"
#include "shaders/ShaderManager.h"


ColoredLineVertex::ColoredLineVertex(glm::vec3 Pos, glm::vec3 Col, uint32_t I)
	: pos(Pos), col(Col), i(I) { }


VertexArrayColoredLines::VertexArrayColoredLines(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& colors, uint32_t id, const std::vector<unsigned>& indices, float lineWidth, bool selectable)
	: m_IndexCount(indices.size()), m_LineWidth(lineWidth), m_Selectable(selectable), m_Model(glm::mat4(1.0)), m_Selected(false)
{

	std::vector<ColoredLineVertex> data;
	for (int i = 0; i < positions.size(); i++) {
		data.emplace_back(positions[i], colors[i], id);
		//  TODO: id is a waste of space, should move to uniform and set durring bind
	}

	GLCall(glGenBuffers(1, &m_VertexBufferID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(ColoredLineVertex) * data.size(), data.data(), GL_STATIC_DRAW));

	GLCall(glGenBuffers(1, &m_IndexBufferID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), indices.data(), GL_STATIC_DRAW));

	GLCall(glGenVertexArrays(1, &m_ID));
	GLCall(glBindVertexArray(m_ID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glEnableVertexAttribArray(3));

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredLineVertex),
		(void*)offsetof(ColoredLineVertex, pos)));
	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredLineVertex),
		(void*)offsetof(ColoredLineVertex, col)));
	GLCall(glVertexAttribIPointer(3, 1, GL_INT, sizeof(ColoredLineVertex),
		(void*)offsetof(ColoredLineVertex, i)));

}

VertexArrayColoredLines::~VertexArrayColoredLines()
{
	GLCall(glDeleteBuffers(1, &m_IndexBufferID));
	GLCall(glDeleteBuffers(1, &m_VertexBufferID));
	GLCall(glDeleteVertexArrays(1, &m_ID));
}

void VertexArrayColoredLines::Render() const {
	ShaderManager::Bind(ShaderProgramType::ColoredLineShader);
	ShaderManager::UpdateLocalUniforms(m_Model, m_Selectable, m_Selected);
	GLCall(glBindVertexArray(m_ID));
	GLCall(glLineWidth(m_LineWidth));
	GLCall(glDrawElements(GL_LINES, GetIndexCount(), GL_UNSIGNED_INT, (GLvoid*)0));
}