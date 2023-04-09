#pragma once

#include "Renderable.h"
#include "../render/VertexArrayColoredLines.h"
#include "../scene/Scene.h"

#include "glm/glm.hpp"

#include <memory>
#include <vector>


class BSpline : public Renderable {

public:


	BSpline(std::vector<glm::vec3> points, unsigned degree, unsigned id = Scene::GetNewID());

	void UpdateLastPoint(glm::vec3 point);

	virtual void Render() const override;
	inline virtual unsigned GetID() const override { return m_ID; }



private:

	glm::vec3 Sample(float t, unsigned degree);
	void SetSamplePoints();
	void UpdateVertexArray();

	std::vector<glm::vec3> m_Points, m_SamplePoints;
	unsigned m_ID, m_Degree;

	std::unique_ptr<VertexArrayColoredLines> m_VertexArray;

};