#pragma once

#include "Renderable.h"
#include "../scene/Scene.h"
#include "../render/VertexArrayBasicLines.h"
#include "../render/VertexArrayBasicPoints.h"
#include "glm/glm.hpp"

#include <vector>
#include <unordered_map>
#include <memory>

class PolyLine : public Renderable {

public:

	PolyLine(const std::vector<glm::vec3>& points, bool dahsed = false, unsigned id = Scene::GetNewID());

	virtual void Render() const override;

	virtual AxisAlignedBoundingBox GetBoundingBox() const override;
	virtual AxisAlignedBoundingBox GetSubSelectionBoundingBox() const override;
	virtual void BakeSelectionTransform(const glm::mat4& t) override;

	virtual glm::vec3 Intersect(Ray r, uint32_t subID) const override;

	virtual void AddSubSelection(uint32_t subID) override;
	virtual void RemoveSubSelection(uint32_t subID) override;
	virtual void ClearSubSelection() override;

	inline bool IsSubSelected() const { return m_VertexSelectionCounter.size(); }

	inline virtual bool IsSelected() const override { return m_Selected; }
	inline virtual void Select() override { m_Selected = true; }
	inline virtual void UnSelect() override { m_Selected = false; }

	void AddPoint(const glm::vec3& point);
	void RemoveLast();

	void UpdateLast(const glm::vec3& point);

	inline size_t GetNumControlPoints() const { return m_Points.size(); }
	inline size_t GetNumSegments() const { return m_Indecies.size() / 2; }
	std::vector<glm::vec3> GetControlPoints(bool withSelectionTransform = true);

	inline virtual unsigned GetID() const override { return m_ID; };

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
	bool m_Dashed;
	std::vector<unsigned> m_Indecies;

	std::vector<uint32_t> m_SegmentSelectionBuffer, m_VertexSelectionBuffer;
	std::unordered_map<int, int> m_VertexSelectionCounter;

	std::vector<glm::vec3> m_Points;
	glm::vec3 m_Color;
	glm::mat4 m_Model;
	std::unique_ptr<VertexArrayBasicLines> m_VertexArrayLines;
	std::unique_ptr<VertexArrayBasicPoints>	m_VertexArrayPoints;

};