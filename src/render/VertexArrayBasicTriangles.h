#pragma once

#include "VertexArray.h"

#include "OpenGLUtils.h"
#include "glm/vec3.hpp"
#include "glm/matrix.hpp"
#include <vector>

struct ColoredTriangleVertex {
	glm::vec3 pos, norm;
};

class VertexArrayBasicTriangles : public VertexArray {

public:

	VertexArrayBasicTriangles(
		const std::vector<glm::vec3>& positions,
		const std::vector<glm::vec3>& normals,
		const std::vector<uint32_t>& triangleSelectionBuffer,
		const std::vector<uint32_t>& vertexSelectionBuffer,
		const glm::vec3& color, uint32_t id, unsigned subIDOffset,
		const std::vector<unsigned>& indecies);

	~VertexArrayBasicTriangles();

	virtual void Render(const glm::mat4& t, unsigned id, bool selectable, bool subSelectable, bool selected) const override;


	// todo: implement partial update
	void UpdateSelectionBuffers(const std::vector<uint32_t>& triangleSelection, const std::vector<uint32_t>& vertexSelection, bool updateSize = false);

	void UpdateVertexOrientation(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals);

private:

	GLuint m_RenderID, m_VertexBufferID, m_IndexBufferID, m_TriangleSelectionBufferID, m_VertexSelectionBufferID;
	unsigned m_IndexCount, m_SubIDOffset;
	glm::vec3 m_Color;

};