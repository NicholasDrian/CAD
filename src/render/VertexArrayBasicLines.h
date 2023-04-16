#pragma once

#include "VertexArray.h"
#include "../render/OpenglUtils.h"

#include "glm/vec3.hpp"
#include "glm/matrix.hpp"
#include <vector>

struct ColoredLineVertex {
	glm::vec3 pos;
};

class VertexArrayBasicLines : public VertexArray {

public:

	VertexArrayBasicLines(const std::vector<glm::vec3>& positions, const glm::vec3& color, uint32_t id, const std::vector<unsigned>& indecies, float lineWidth, bool subSelectable = false, const std::vector<uint32_t>& subSelectionBuffer = {}, const std::vector<uint32_t>& segmentSelectionBuffer = {});

	~VertexArrayBasicLines();

	virtual void Render(const glm::mat4& t, unsigned id, bool selectable, bool subSelectable, bool selected) const override;
	virtual PrimitiveType GetPrimitiveType() const override { return PrimitiveType::Line; }
	virtual unsigned GetIndexCount() const override { return m_IndexCount; };

	void UpdateVertexPositions(const std::vector<glm::vec3>& positions);

	//todo parital update
	void UpdateSegmentSelectionBuffer(const std::vector<uint32_t>& segmentSelection, const std::vector<uint32_t>& vertexSelection, bool updateSize = false);

private:

	GLuint m_RenderID, m_VertexBufferID, m_IndexBufferID, 
		m_SegmentSelectionBufferID, m_VertexSelectionBufferID;
	unsigned m_IndexCount;
	glm::vec3 m_Color;
	GLfloat m_LineWidth;

};