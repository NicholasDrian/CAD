#pragma once

#include "../render/VertexArrayScreenSpaceTriangles.h"
#include "../geometry/Frustum.h"

#include <unordered_set>
#include <memory>

class SelectionRectangle {

public:

	SelectionRectangle(int left, int top, int right, int bottom);

	Frustum GetFrustum() const;

	void Update(int left, int top, int right, int bottom);
	void Render() const;

private:

	void UpdateVertexArray();

	int m_Left, m_Top, m_Right, m_Bottom;

	std::unique_ptr<VertexArrayScreenSpaceTriangles> m_VertexArray;

};