#pragma once 

#include "VertexArrayBasicLines.h"
#include "shaders/ShaderManager.h"

#include <iostream>



VertexArrayBasicLines::VertexArrayBasicLines(const std::vector<glm::vec3>& positions, const glm::vec3& color, uint32_t id, const std::vector<unsigned>& indices, float lineWidth, unsigned subIDOffset, bool subSelectable, bool dashed, const std::vector<uint32_t>& segmentSelectionBuffer, const std::vector<uint32_t>& vertexSelectionBuffer)
	: m_IndexCount((unsigned)indices.size()), m_LineWidth(lineWidth), m_Color(color), m_SubIDOffset(subIDOffset), m_Dashed(dashed)
{
	if (subSelectable) {
		GLCall(glGenBuffers(1, &m_SegmentSelectionBufferID));
		GLCall(glGenBuffers(1, &m_VertexSelectionBufferID));
		UpdateSegmentSelectionBuffer(segmentSelectionBuffer, vertexSelectionBuffer, true);
	}
	else {
		m_SegmentSelectionBufferID = 0;
		m_VertexSelectionBufferID = 0;
	}

	
	GLCall(glGenBuffers(1, &m_VertexBufferID));
	UpdateVertexPositions(positions);

	GLCall(glGenVertexArrays(1, &m_RenderID));
	GLCall(glBindVertexArray(m_RenderID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));

	GLCall(glGenBuffers(1, &m_IndexBufferID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), indices.data(), GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BasicLineVertex),
		(void*)offsetof(BasicLineVertex, pos)));

	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BasicLineVertex),
		(void*)offsetof(BasicLineVertex, tan)));

	GLCall(glBindVertexArray(0));
}

void VertexArrayBasicLines::UpdateVertexPositions(const std::vector<glm::vec3>& positions)
{
	std::vector<BasicLineVertex> data(positions.size());
	for (int i = 0; i < positions.size() - 1; i++) {
		data[i] = {positions[i], positions[i] - positions[i + 1]};
	}
	data.back() = {positions.back(), {1.0,0.0,0.0}};
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(BasicLineVertex) * data.size(), data.data(), GL_STATIC_DRAW));
}

VertexArrayBasicLines::~VertexArrayBasicLines()
{
	GLCall(glDeleteBuffers(1, &m_IndexBufferID));
	GLCall(glDeleteBuffers(1, &m_VertexBufferID));
	GLCall(glDeleteBuffers(1, &m_SegmentSelectionBufferID));
	GLCall(glDeleteBuffers(1, &m_VertexSelectionBufferID));
	GLCall(glDeleteVertexArrays(1, &m_RenderID));
}

void VertexArrayBasicLines::Render(const glm::mat4& model, unsigned id, bool selectable, bool subSelectable, bool selected) const {
	if (m_Dashed) ShaderManager::Bind(ShaderProgramType::DashedLineShader);
	else ShaderManager::Bind(ShaderProgramType::BasicLineShader);
	ShaderManager::UpdateLocalUniforms(model, m_Color, selectable, subSelectable, selected, id, m_SubIDOffset);
	GLCall(glBindVertexArray(m_RenderID));
	if (subSelectable) {
		GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_VertexSelectionBufferID));
		GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_SegmentSelectionBufferID));
	}
	GLCall(glLineWidth(m_LineWidth));
	GLCall(glDrawElements(GL_LINES, m_IndexCount, GL_UNSIGNED_INT, (GLvoid*)0));
}


void VertexArrayBasicLines::UpdateSegmentSelectionBuffer(const std::vector<uint32_t>& segmentSelection, const std::vector<uint32_t>& vertexSelection, bool updateSize)
{
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SegmentSelectionBufferID));
	if (updateSize) GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * segmentSelection.size(), segmentSelection.data(), GL_STATIC_DRAW));
	else			GLCall(glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(uint32_t) * segmentSelection.size(), segmentSelection.data()));

	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_VertexSelectionBufferID));
	if (updateSize) GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * vertexSelection.size(), vertexSelection.data(), GL_STATIC_DRAW));
	else			GLCall(glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(uint32_t) * vertexSelection.size(), vertexSelection.data()));

}