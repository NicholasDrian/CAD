#pragma once

#include <vector>
#include <memory>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

enum class PrimitiveType {
	Triangles,
	Lines,
	Points
};

class VertexArray {

public:

	VertexArray(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, PrimitiveType type);
	~VertexArray();

	inline PrimitiveType GetPrimitiveType() const { return m_PrimitiveType; }
	inline size_t GetVertexCount() const { return m_VertexBuffer->GetVertexCount(); }
	inline size_t GetIndexCount() const { return m_IndexBuffer? m_IndexBuffer->GetIndexCount() : 0; }

	void Bind() const;

private:

	GLuint m_ID;
	PrimitiveType m_PrimitiveType;

	std::shared_ptr<VertexBuffer> m_VertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;

};