#pragma once

#include "Renderable.h"
#include "../scene/Scene.h"
#include "../render/VertexArrayColoredLines.h"
#include "glm/glm.hpp"

#include <vector>
#include <memory>

class PolyLine : public Renderable {

public:

	PolyLine(const std::vector<glm::vec3>& points, unsigned id = Scene::GetNewID());

	virtual void Render() const override;

	void AddPoint(const glm::vec3& point);
	void RemoveLast();

	void UpdateLast(const glm::vec3& point);

	inline virtual unsigned GetID() const override { return m_ID; };


private:

	void UpdateVertexArray();

	unsigned m_ID;
	std::vector<unsigned> m_Indecies;
	std::vector<glm::vec3> m_Points, m_Colors;
	std::unique_ptr<VertexArrayColoredLines> m_VertexArray;
	

};