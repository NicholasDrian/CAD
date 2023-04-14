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

	VertexArrayBasicTriangles(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<uint32_t>& triangleSelectionBuffer, const std::vector<uint32_t>& vertexSelectionBuffer, const glm::vec3& color, uint32_t id, const std::vector<unsigned>& indecies);

	~VertexArrayBasicTriangles();

	virtual void Render(unsigned id, bool selectable, bool subSelectable, bool selected) const override;
	virtual PrimitiveType GetPrimitiveType() const override { return PrimitiveType::Triangle; }
	virtual unsigned GetIndexCount() const override { return m_IndexCount; };

	// todo
	//void UpdatePrimitiveSelectionBuffer(unsigned index, uint32_t val);
	//void UpdateVertexSelectionBuffer(unsigned index, uint32_t val);
	void UpdateSelectionBuffers(const std::vector<uint32_t>& primitiveSelection, const std::vector<uint32_t>& vertexSelection, bool updateSize = false);

private:

	GLuint m_RenderID, m_VertexBufferID, m_IndexBufferID, 
		m_TriangleSelectionBufferID, m_VertexSelectionBufferID;
	unsigned m_IndexCount;
	glm::mat4 m_Model;
	glm::vec3 m_Color;

};