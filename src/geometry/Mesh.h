#pragma once

#include "Renderable.h"
#include "../render/VertexArrayColoredTriangle.h"
#include "../scene/Scene.h"
#include "glm/vec3.hpp"

#include <memory>

class Mesh : public Renderable {

public:

	Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<glm::vec3>& colors, const std::vector<unsigned>& indecies, unsigned id = Scene::GetNewID(), bool selectable = true);

	virtual void Render() const override;

	inline virtual unsigned GetID() const override { return m_ID; };

private:

	uint32_t m_ID;

	std::vector<glm::vec3> m_Positions, m_Normals, m_Colors;

	std::vector<unsigned> m_Indecies;

	VertexArrayColoredTriangle m_VertexArray;

};