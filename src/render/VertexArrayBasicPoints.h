#pragma once

#include "VertexArray.h"

#include "glm/glm.hpp"
#include "GL/glew.h"

#include <vector>


//todo!
class VertexArrayBasicPoints : public VertexArray {

public:

	VertexArrayBasicPoints(const std::vector<glm::vec3>& points, const std::vector<uint32_t>& subSelection, const glm::vec4& color, unsigned subIDOffset);
	VertexArrayBasicPoints(const std::vector<glm::vec4>& points, const std::vector<uint32_t>& subSelection, const glm::vec4& color, unsigned subIDOffset);

	~VertexArrayBasicPoints();

	virtual void Render(const glm::mat4& t, unsigned id, bool selectable, bool subSelectable, bool selected) const override;

	void UpdateSubSelection(const std::vector<uint32_t>& subSelection);
	void UpdatePositions(const std::vector<glm::vec3>& positions, bool updateSize = false);

private:

	void Init(const std::vector<glm::vec3>& points, const std::vector<uint32_t>& subSelection, const glm::vec4& color, unsigned subIDOffset);

	GLuint m_ID, m_PointBuffer, m_SubSelectionBuffer;
	glm::vec4 m_Color;
	unsigned m_PointCount, m_SubIDOffset;

};