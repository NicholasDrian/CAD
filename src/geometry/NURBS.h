#pragma once 

#include "Renderable.h"
#include "../render/VertexArrayBasicLines.h"
#include "../render/VertexArrayBasicPoints.h"
#include "PolyLine.h"
#include "../scene/Scene.h"

#include "glm/glm.hpp"

#include <vector>
#include <memory>

class NURBS : public Renderable {

public:

	NURBS(std::vector<glm::vec3> points, glm::vec4 color, std::vector<float> weights = {}, unsigned degree = 2U, std::vector<float> knots = {}, unsigned id = Scene::GetNewID());

	virtual void Render() const override;

	virtual AxisAlignedBoundingBox GetBoundingBox() const override;
	virtual AxisAlignedBoundingBox GetSubSelectionBoundingBox() const override;
	virtual void BakeSelectionTransform(const glm::mat4& t) override;

	virtual void AddSubSelection(uint32_t subID) override;
	virtual void RemoveSubSelection(uint32_t subID) override;
	virtual void ClearSubSelection() override;

	virtual bool IsSelected() const override { return m_Selected; }
	virtual void Select() override;
	virtual void UnSelect() override;

	virtual void SelectionTransformUpdated() override;

	virtual glm::vec3 Intersect(Ray r, uint32_t subID) const override;

	inline virtual unsigned GetID() const override { return m_ID; }
	inline size_t GetNumControlPoints() const { return m_Points.size(); };
	inline unsigned GetDegree() const { return m_Degree; }
	inline const std::vector<glm::vec4>& GetControlPoints() const { return m_Points; }
	inline std::vector<glm::vec4>& GetControlPoints() { return m_Points; }

	virtual void PointsOn() override { m_PointsOn = true; }
	virtual void PointsOff() override { m_PointsOn = false; }

	void AddControlPoint(const glm::vec3& point, bool incrementDegree = false);
	void ChangeDegree(unsigned degree);

	void UpdateLastPoint(const glm::vec3& point);
	void RemoveLastPoint();

private:

	bool m_Selected;
	bool m_PointsOn;

	std::vector<float> BasisFuncs(float u) const;
	int KnotSpan(float u) const;
	glm::vec3 Sample(float t) const;

	void UpdateKnotVector();
	void UpdateSamples();
	void UpdateVertexArray();

	glm::mat4 m_Model;
	std::vector<glm::vec4> m_Points;
	glm::vec4 m_Color;
	std::vector<float> m_Knots;
	std::vector<unsigned> m_Indecies;
	unsigned m_Degree;
	unsigned m_ID;

	std::vector<glm::vec3> m_Samples;

	std::unique_ptr<VertexArrayBasicLines> m_VertexArrayLines;
	std::unique_ptr<PolyLine> m_ControlPolyLine;

};