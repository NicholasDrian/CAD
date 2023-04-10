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
	inline size_t GetNumControlPoints() const { return m_Points.size(); };
	inline unsigned GetDegree() const { return m_Degree; }
	inline const std::vector<glm::vec3>& GetControlPoints() const { return m_Points; }
	inline std::vector<glm::vec3>& GetControlPoints() { return m_Points; }

	void AddControlPoint(glm::vec3 point, bool incrementDegree = false);
	void ChangeDegree(unsigned degree);

	void UpdateLastPoint(glm::vec3 point);
	void RemoveLastPoint();

private:

	std::vector<float> BasisFuncs(float u) const;
	int KnotSpan(float u) const;
	glm::vec3 Sample(float t) const;

	void UpdateKnotVector();
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