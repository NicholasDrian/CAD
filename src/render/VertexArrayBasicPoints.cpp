#pragma once

#include "VertexArrayBasicPoints.h"
#include "OpenGLUtils.h"
#include "shaders/ShaderManager.h"

#include <iostream>

VertexArrayBasicPoints::VertexArrayBasicPoints(const std::vector<glm::vec3>& points, const std::vector<uint32_t>& subSelection, const glm::vec3& color, unsigned subIDOffset)
{
	Init(points, subSelection, color, subIDOffset);
}

VertexArrayBasicPoints::VertexArrayBasicPoints(const std::vector<glm::vec4>& points, const std::vector<uint32_t>& subSelection, const glm::vec3& color, unsigned subIDOffset)
{
	std::vector<glm::vec3> point3s(points.size());
	for (int i = 0; i < points.size(); i++) point3s[i] = points[i];
	Init(point3s, subSelection, color, subIDOffset);
}

void VertexArrayBasicPoints::Init(const std::vector<glm::vec3>& points, const std::vector<uint32_t>& subSelection, const glm::vec3& color, unsigned subIDOffset) 
{
	m_SubIDOffset = subIDOffset;
	m_Color = color;

	GLCall(glGenBuffers(1, &m_PointBuffer));
	UpdatePositions(points, true);

	GLCall(glGenBuffers(1, &m_SubSelectionBuffer));
	UpdateSubSelection(subSelection);

	GLCall(glGenVertexArrays(1, &m_ID));
	GLCall(glBindVertexArray(m_ID));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_PointBuffer));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0));

	GLCall(glBindVertexArray(0));
}

VertexArrayBasicPoints::~VertexArrayBasicPoints()
{
	GLCall(glDeleteVertexArrays(1, &m_ID));
	GLCall(glDeleteBuffers(1, &m_PointBuffer));
	GLCall(glDeleteBuffers(1, &m_SubSelectionBuffer));
}

void VertexArrayBasicPoints::Render(const glm::mat4& model, unsigned id, bool selectable, bool subSelectable, bool selected) const
{
	ShaderManager::Bind(ShaderProgramType::BasicPointShader);
	ShaderManager::UpdateLocalUniforms(model, m_Color, selectable, subSelectable, selected, id, m_SubIDOffset);
	if (subSelectable) {
		GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_SubSelectionBuffer));
	}

	GLCall(glBindVertexArray(m_ID));
	GLCall(glDrawArrays(GL_POINTS, 0, m_PointCount));
}

void VertexArrayBasicPoints::UpdateSubSelection(const std::vector<uint32_t>& subSelection)
{
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SubSelectionBuffer));
	GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, subSelection.size() * sizeof(uint32_t), subSelection.data(), GL_STATIC_DRAW));
}

void VertexArrayBasicPoints::UpdatePositions(const std::vector<glm::vec3>& positions, bool updateSize)
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_PointBuffer));
	if (updateSize) {
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW));
		m_PointCount = positions.size();
	}
	else GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * positions.size(), positions.data()));
}
