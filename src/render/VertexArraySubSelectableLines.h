#pragma once

#include "VertexArrayLines.h"

class VertexArraySubSelectableLines : public VertexArrayLines {

public:

	VertexArraySubSelectableLines(const std::vector<glm::vec3>& positions, const glm::vec4& color, uint32_t id, const std::vector<unsigned>& indecies, float lineWidth, bool dashed = false, const std::vector<uint32_t>& vertexSelectionBuffer = {}, const std::vector<uint32_t>& segmentSelectionBuffer = {});

	void Render(const glm::mat4& t, unsigned id, bool selectable, bool selected) const;

	virtual ~VertexArraySubSelectableLines() override;

	//todo parital update
	void UpdateSegmentSelectionBuffer(const std::vector<uint32_t>& segmentSelection, const std::vector<uint32_t>& vertexSelection, bool updateSize = false);

private:

	GLuint m_SegmentSelectionBufferID, m_VertexSelectionBufferID;

};