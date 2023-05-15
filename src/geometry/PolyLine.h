#pragma once

#include "Renderable.h"
#include "../scene/Scene.h"
#include "../render/VertexArraySubSelectableLines.h"
#include "../render/VertexArrayPoints.h"
#include "glm/glm.hpp"

#include <vector>
#include <unordered_map>
#include <memory>

class PolyLine : public Renderable {

public:

	PolyLine(const std::vector<glm::vec3>& points, bool dahsed = false, unsigned id = Scene::GetNewID());

	virtual void Render() const override;

	virtual AxisAlignedBoundingBox GetBoundingBox() const override;
	virtual AxisAlignedBoundingBox GetBoundingBoxLocalSpace(uint32_t subID) const override;
	virtual AxisAlignedBoundingBox GetSubSelectionBoundingBox() const override;


	virtual void SelectWithinFrustum(const Frustum& frustum, bool inclusive) override;
	virtual void SubSelectWithinFrustum(const Frustum& frustum, bool inclusive) override;
	virtual void UnSelectWithinFrustum(const Frustum& frustum, bool inclusive) override;
	virtual void UnSubSelectWithinFrustum(const Frustum& frustum, bool inclusive) override;

	virtual void BakeSelectionTransform(const glm::mat4& t) override;

	virtual glm::vec3 Intersect(Ray r, uint32_t subID) const override;
	virtual bool IntersectsLocalSpace(Ray r, uint32_t subID, float MaxDistancePixels) const override;

	virtual void AddSubSelection(uint32_t subID) override;
	virtual void RemoveSubSelection(uint32_t subID) override;
	virtual void ClearSubSelection() override;

	inline virtual void PointsOn() override { m_PointsOn = true; }
	inline virtual void PointsOff() override { m_PointsOn = false; }

	inline virtual bool IsSelected() const override { return m_Selected; }
	inline virtual void Select() override { m_Selected = true; }
	inline virtual void UnSelect() override { m_Selected = false; }
	inline bool IsSubSelected() const { return m_VertexSelectionCounter.size(); }
	
	inline virtual const glm::mat4& GetModel() const override { return m_Model; }

	void AddPoint(const glm::vec3& point);
	void RemoveLast();
	void UpdateLast(const glm::vec3& point);

	inline size_t GetNumControlPoints() const { return m_Points.size(); }
	inline size_t GetNumSegments() const { return m_Indecies.size() / 2; }
	std::vector<glm::vec3> GetControlPoints(bool withSelectionTransform = true);

	inline virtual unsigned GetID() const override { return m_ID; };

	inline void SetModel(const glm::mat4& model) { m_Model = model; }

private:

	bool m_Selected;

	void AddSubSelectionLine(uint32_t subID);
	void AddSubSelectionPoint(uint32_t subID);
	void RemoveSubSelectionLine(uint32_t subID);
	void RemoveSubSelectionPoint(uint32_t subID);

	void UpdateVertexArrays();
	void UpdatePositions();
	void UpdateSubSelections();

	unsigned m_ID;
	bool m_Dashed, m_PointsOn;
	std::vector<unsigned> m_Indecies;

	std::vector<uint32_t> m_SegmentSelectionBuffer, m_VertexSelectionBuffer;
	std::unordered_map<int, int> m_VertexSelectionCounter;

	std::vector<glm::vec3> m_Points;
	glm::vec4 m_Color;
	glm::mat4 m_Model;
	std::unique_ptr<VertexArraySubSelectableLines> m_VertexArrayLines;
	std::unique_ptr<VertexArrayPoints>	m_VertexArrayPoints;

};