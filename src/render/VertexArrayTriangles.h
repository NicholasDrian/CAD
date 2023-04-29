#pragma once


#include "OpenGLUtils.h"
#include "glm/vec3.hpp"
#include "glm/matrix.hpp"
#include <vector>

struct ColoredTriangleVertex {
	glm::vec3 pos, norm;
};

class VertexArrayTriangles {

public:

	VertexArrayTriangles( const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const glm::vec4& color, const std::vector<unsigned>& indices);

	virtual ~VertexArrayTriangles();

	void Render(const glm::mat4& t, unsigned id, bool selectable, bool selected) const;

	void UpdateVertexOrientation(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals);

protected:

	GLuint m_RenderID, m_VertexBufferID, m_IndexBufferID;
	unsigned m_IndexCount;
	glm::vec4 m_Color;

};