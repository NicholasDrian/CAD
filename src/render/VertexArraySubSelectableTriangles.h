#pragma once

#include "VertexArrayTriangles.h"

class VertexArraySubSelectableTriangles : public VertexArrayTriangles {

public:

	VertexArraySubSelectableTriangles(
		const std::vector<glm::vec3>& positions,
		const std::vector<glm::vec3>& normals,
		const std::vector<uint32_t>& triangleSelectionBuffer,
		const std::vector<uint32_t>& vertexSelectionBuffer,
		const glm::vec4& color, uint32_t id,
		const std::vector<unsigned>& indices);

	virtual ~VertexArraySubSelectableTriangles() override;

	void Render(const glm::mat4& t, unsigned id, bool selectable, bool selected) const;


	// todo: implement partial update
	void UpdateSelectionBuffers(const std::vector<uint32_t>& triangleSelection, const std::vector<uint32_t>& vertexSelection, bool updateSize = false);

private:

	GLuint m_TriangleSelectionBufferID, m_VertexSelectionBufferID;

};