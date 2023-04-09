#pragma once 

#include "Renderable.h"
#include "../render/VertexArrayColoredLines.h"
#include "../scene/Scene.h"
#include "glm/glm.hpp"

#include <vector>
#include <memory>

class NURBS : public Renderable {

public:

	NURBS(std::vector<glm::vec3> points, std::vector<float> weights = {}, unsigned degree = 2U, std::vector<float> knots = {}, unsigned id = Scene::GetNewID());

	virtual void Render() const override;
	inline virtual unsigned GetID() const override { return m_ID; }

	void UpdateLastPoint(glm::vec3 point);

private:

	std::vector<float> BasisFuncs(float u) const;
	int KnotSpan(float u) const;
	glm::vec3 Sample(float t) const;
	void UpdateSamples();

	void UpdateVertexArray();

	std::vector<glm::vec3> m_Points, m_Colors;
	std::vector<float> m_Weights, m_Knots;
	std::vector<unsigned> m_Indecies;
	unsigned m_Degree;
	unsigned m_ID;

	std::vector<glm::vec3> m_Samples;

	std::unique_ptr<VertexArrayColoredLines> m_VertexArray;

};