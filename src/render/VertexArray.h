#pragma once

#include <vector>

#include "Vertex.h"

enum class PrimitiveType {
	Triangles,
	Lines,
	Points
};

class VertexArray {

public:

	VertexArray(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices, PrimitiveType type);
	~VertexArray();

	inline PrimitiveType GetPrimitiveType() const { return m_PrimitiveType; }
	inline int GetVertexCount() const { return m_VertCount; }
	inline int GetIndexCount() const { return m_IndexCount; }

	void Bind() const;

private:

	GLuint m_ID;
	unsigned m_VertCount, m_IndexCount;
	PrimitiveType m_PrimitiveType;

};