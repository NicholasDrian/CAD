#pragma once 

#include "VertexArrayLines.h"
#include "shaders/ShaderManager.h"

#include <iostream>



VertexArrayLines::VertexArrayLines(const std::vector<glm::vec3>& positions, const glm::vec4& color, uint32_t id, const std::vector<unsigned>& indices, float lineWidth, bool dashed)
	: m_IndexCount((unsigned)indices.size()), m_LineWidth(lineWidth), m_Color(color), m_Dashed(dashed)
{

	
	GLCall(glGenBuffers(1, &m_VertexBufferID));
	UpdateVertexPositions(positions);

	GLCall(glGenVertexArrays(1, &m_RenderID));
	GLCall(glBindVertexArray(m_RenderID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));

	GLCall(glGenBuffers(1, &m_IndexBufferID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), indices.data(), GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex),
		(void*)offsetof(LineVertex, pos)));

	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex),
		(void*)offsetof(LineVertex, tan)));

	GLCall(glBindVertexArray(0));
}

void VertexArrayLines::UpdateVertexPositions(const std::vector<glm::vec3>& positions)
{
	std::vector<LineVertex> data(positions.size());
	for (int i = 0; i < positions.size() - 1; i++) {
		data[i] = {positions[i], positions[i] - positions[i + 1]};
	}
	data.back() = { positions.back(), { 1.0f, 0.0f, 0.0f } };
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(LineVertex) * data.size(), data.data(), GL_STATIC_DRAW));
}

VertexArrayLines::~VertexArrayLines()
{
	GLCall(glDeleteBuffers(1, &m_IndexBufferID));
	GLCall(glDeleteBuffers(1, &m_VertexBufferID));
	GLCall(glDeleteVertexArrays(1, &m_RenderID));
}

void VertexArrayLines::Render(const glm::mat4& model, unsigned id, bool selectable, bool selected) const {

	if (m_Dashed) GLCall(glEnable(GL_LINE_STIPPLE));
	ShaderManager::Bind(ShaderProgramType::LineShader);
	ShaderManager::UpdateLocalUniforms(model, m_Color, selectable, false, selected, id);
	GLCall(glBindVertexArray(m_RenderID));
	GLCall(glLineWidth(m_LineWidth));
	GLCall(glDrawElements(GL_LINES, m_IndexCount, GL_UNSIGNED_INT, (GLvoid*)0));
	GLCall(glDisable(GL_LINE_STIPPLE));
}


