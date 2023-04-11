#pragma once

#include "Renderable.h"
#include "../render/VertexArrayBasicTriangles.h"
#include "../scene/Scene.h"
#include "glm/vec3.hpp"

#include <memory>

class Mesh : public Renderable {

public:

	Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const glm::vec3& color, const std::vector<unsigned>& indecies, unsigned id = Scene::GetNewID(), bool selectable = true);

	virtual void Render() const override;

	inline virtual unsigned GetID() const override { return m_ID; };

private:

	uint32_t m_ID;

	std::vector<glm::vec3> m_Positions, m_Normals;
	glm::vec3 m_Color;

	std::vector<unsigned> m_Indecies;

	VertexArrayBasicTriangles m_VertexArray;

};