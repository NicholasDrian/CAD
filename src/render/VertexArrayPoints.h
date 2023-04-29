#pragma once

#include "glm/glm.hpp"
#include "GL/glew.h"

#include <vector>


//todo!
class VertexArrayPoints {

public:

	VertexArrayPoints(const std::vector<glm::vec3>& points, const std::vector<uint32_t>& subSelection, const glm::vec4& color, unsigned subIDOffset);
	VertexArrayPoints(const std::vector<glm::vec4>& points, const std::vector<uint32_t>& subSelection, const glm::vec4& color, unsigned subIDOffset);

	~VertexArrayPoints();

	void Render(const glm::mat4& t, unsigned id, bool selectable, bool subSelectable, bool selected) const;

	void UpdateSubSelection(const std::vector<uint32_t>& subSelection);
	void UpdatePositions(const std::vector<glm::vec3>& positions, bool updateSize = false);

private:

	void Init(const std::vector<glm::vec3>& points, const std::vector<uint32_t>& subSelection, const glm::vec4& color, unsigned subIDOffset);

	GLuint m_ID, m_PointBuffer, m_SubSelectionBuffer;
	glm::vec4 m_Color;
	unsigned m_PointCount, m_SubIDOffset;

};