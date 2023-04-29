#pragma once

#include "VertexArraySubSelectableLines.h"

#include "shaders/ShaderManager.h"



VertexArraySubSelectableLines::VertexArraySubSelectableLines(const std::vector<glm::vec3>& positions, const glm::vec4& color, uint32_t id, const std::vector<unsigned>& indices, float lineWidth, bool dashed, const std::vector<uint32_t>& vertexSelectionBuffer, const std::vector<uint32_t>& segmentSelectionBuffer)
	: VertexArrayLines(positions, color, id, indices, lineWidth, dashed)
{
	GLCall(glGenBuffers(1, &m_SegmentSelectionBufferID));
	GLCall(glGenBuffers(1, &m_VertexSelectionBufferID));
	UpdateSegmentSelectionBuffer(segmentSelectionBuffer, vertexSelectionBuffer, true);
}

void VertexArraySubSelectableLines::Render(const glm::mat4& model, unsigned id, bool selectable, bool selected) const {

	if (m_Dashed) GLCall(glEnable(GL_LINE_STIPPLE));
	ShaderManager::Bind(ShaderProgramType::LineShader);
	ShaderManager::UpdateLocalUniforms(model, m_Color, selectable, true, selected, id);
	GLCall(glBindVertexArray(m_RenderID));
	GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_VertexSelectionBufferID));
	GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_SegmentSelectionBufferID));
	GLCall(glLineWidth(m_LineWidth));
	GLCall(glDrawElements(GL_LINES, m_IndexCount, GL_UNSIGNED_INT, (GLvoid*)0));
	GLCall(glDisable(GL_LINE_STIPPLE));
}

VertexArraySubSelectableLines::~VertexArraySubSelectableLines()
{
	VertexArrayLines::~VertexArrayLines();
	GLCall(glDeleteBuffers(1, &m_SegmentSelectionBufferID));
	GLCall(glDeleteBuffers(1, &m_VertexSelectionBufferID));
}

void VertexArraySubSelectableLines::UpdateSegmentSelectionBuffer(const std::vector<uint32_t>& segmentSelection, const std::vector<uint32_t>& vertexSelection, bool updateSize)
{
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SegmentSelectionBufferID));
	if (updateSize) GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * segmentSelection.size(), segmentSelection.data(), GL_STATIC_DRAW));
	else			GLCall(glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(uint32_t) * segmentSelection.size(), segmentSelection.data()));

	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_VertexSelectionBufferID));
	if (updateSize) GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * vertexSelection.size(), vertexSelection.data(), GL_STATIC_DRAW));
	else			GLCall(glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(uint32_t) * vertexSelection.size(), vertexSelection.data()));
}