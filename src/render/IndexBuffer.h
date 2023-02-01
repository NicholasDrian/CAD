#pragma once

#include <vector>
#include "GL/glew.h"

class IndexBuffer {

public:

	IndexBuffer(const std::vector<unsigned>& indices);
	~IndexBuffer();

	void Bind();

	inline size_t GetIndexCount() const { return m_IndexCount; }

private:

	GLuint m_ID;
	size_t m_IndexCount;

};