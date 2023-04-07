
#pragma once

enum class PrimitiveType {
	None = 0,
	Triangle,
	Line,
	Point
};

class VertexArray {

public:

	virtual PrimitiveType GetPrimitiveType() const = 0;
	virtual unsigned GetIndexCount() const = 0;
	virtual void Render(unsigned id, bool selectable, bool selected) const = 0;

};