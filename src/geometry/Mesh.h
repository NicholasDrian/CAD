#pragma once

#include "Renderable.h"
#include "../render/VertexArrayBasicTriangles.h"
#include "../scene/Scene.h"
#include "glm/vec3.hpp"

#include <memory>

class Mesh : public Renderable {

public:

	Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const glm::vec3& color, const std::vector<unsigned>& indecies, unsigned id = Scene::GetNewID());

	virtual void Render() const override;

	virtual AxisAlignedBoundingBox GetBoundingBox() const override;
	virtual AxisAlignedBoundingBox GetSubSelectionBoundingBox() const override;
	virtual void BakeSelectionTransform(const glm::mat4& t) override;

	virtual glm::vec3 Intersect(Ray r, uint32_t subID) const override;

	virtual void AddSubSelection(uint32_t subID) override;
	virtual void RemoveSubSelection(uint32_t subID) override;
	virtual void ClearSubSelection() override;

	inline virtual unsigned GetID() const override { return m_ID; };

private:

	uint32_t m_ID;

	std::vector<glm::vec3> m_Positions, m_Normals;
	glm::vec3 m_Color;
	glm::mat4 m_Model;

	std::vector<uint32_t> m_SelectedTriangles;
	std::unordered_map<int, int> m_SelectedVertexCounter;
	std::vector<uint32_t> m_SelectedVertices;

	std::vector<unsigned> m_Indices;

	std::unique_ptr<VertexArrayBasicTriangles> m_VertexArray;

};