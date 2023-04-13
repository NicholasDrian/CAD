#pragma once

#include "Renderable.h"
#include "../scene/Scene.h"
#include "../render/VertexArrayBasicLines.h"
#include "glm/glm.hpp"

#include <vector>
#include <unordered_map>
#include <memory>

class PolyLine : public Renderable {

public:

	PolyLine(const std::vector<glm::vec3>& points, unsigned id = Scene::GetNewID());

	virtual void Render() const override;

	virtual void AddSubSelection(uint32_t subID) override;
	virtual void RemoveSubSelection(uint32_t subID) override;
	virtual void ClearSubSelection() override;

	void AddPoint(const glm::vec3& point);
	void RemoveLast();

	void UpdateLast(const glm::vec3& point);

	inline size_t GetNumControlPoints() const { return m_Points.size(); }

	inline virtual unsigned GetID() const override { return m_ID; };


private:

	void UpdateVertexArray();

	unsigned m_ID;
	std::vector<unsigned> m_Indecies;

	std::vector<uint32_t> m_SubSelectionBuffer;

	std::vector<glm::vec3> m_Points;
	glm::vec3 m_Color;
	std::unique_ptr<VertexArrayBasicLines> m_VertexArray;
	

};