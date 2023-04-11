#pragma once 

#include "VertexArrayBasicLines.h"
#include "shaders/ShaderManager.h"



VertexArrayBasicLines::VertexArrayBasicLines(const std::vector<glm::vec3>& positions, const glm::vec3& color, uint32_t id, const std::vector<unsigned>& indices, float lineWidth)
	: m_IndexCount((unsigned)indices.size()), m_LineWidth(lineWidth), m_Model(glm::mat4(1.0)), m_Color(color)
{

	std::vector<ColoredLineVertex> data;
	for (int i = 0; i < positions.size(); i++) {
		data.emplace_back(positions[i]);
	}

	GLCall(glGenVertexArrays(1, &m_RenderID));
	GLCall(glBindVertexArray(m_RenderID));

	GLCall(glGenBuffers(1, &m_VertexBufferID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(ColoredLineVertex) * data.size(), data.data(), GL_STATIC_DRAW));

	GLCall(glGenBuffers(1, &m_IndexBufferID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), indices.data(), GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredLineVertex),
		(void*)offsetof(ColoredLineVertex, pos)));

}

VertexArrayBasicLines::~VertexArrayBasicLines()
{
	GLCall(glDeleteBuffers(1, &m_IndexBufferID));
	GLCall(glDeleteBuffers(1, &m_VertexBufferID));
	GLCall(glDeleteVertexArrays(1, &m_RenderID));
}

void VertexArrayBasicLines::Render(unsigned id, bool selectable, bool selected) const {
	ShaderManager::Bind(ShaderProgramType::BasicLineShader);
	ShaderManager::UpdateLocalUniforms(m_Model, m_Color, selectable, selected, id);
	GLCall(glBindVertexArray(m_RenderID));
	GLCall(glLineWidth(m_LineWidth));
	GLCall(glDrawElements(GL_LINES, GetIndexCount(), GL_UNSIGNED_INT, (GLvoid*)0));
}