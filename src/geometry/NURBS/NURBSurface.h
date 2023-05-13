#pragma once

#include "../Renderable.h"

#include "../../render/VertexArrayTriangles.h"

#include <memory>

class NURBSurface : public Renderable {

public:

	NURBSurface(
		const std::vector<std::vector<glm::vec4>>& points,
		const std::vector<float>& knotsU,
		const std::vector<float>& knotsV,
		int degreeU, int degreeV);

	virtual void Render() const override;

	inline virtual unsigned GetID() const override { return m_ID; }

	virtual AxisAlignedBoundingBox GetBoundingBox() const override;
	virtual AxisAlignedBoundingBox GetSubSelectionBoundingBox() const override;

	virtual void BakeSelectionTransform(const glm::mat4& t) override;

	virtual glm::vec3 Intersect(Ray r, uint32_t subID) const override;

	inline virtual bool IsSelected() const override { return m_Selected; }
	inline virtual void Select() override { m_Selected = true; }
	inline virtual void UnSelect() override { m_Selected = false; }

private:

	void ControlPointsUpdated();
	glm::vec3 Sample(float u, float v) const;

	std::vector<std::vector<glm::vec4>> m_Points;
	std::vector<std::vector<glm::vec3>> m_Samples;
	std::vector<float> m_KnotsU;
	std::vector<float> m_KnotsV;
	int m_DegreeU, m_DegreeV;

	glm::mat4 m_Model;

	std::unique_ptr<VertexArrayTriangles> m_VertexArray;

	bool m_Selected;
	unsigned m_ID;

};