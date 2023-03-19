#pragma once

#include "Renderable.h"
#include "../render/VertexArrayColoredTriangle.h"

#include "glm/vec3.hpp"

#include <memory>

class Mesh : public Renderable {

public:

	Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<glm::vec3>& colors, uint32_t id, const std::vector<unsigned>& indecies);

	// TODO
	//Mesh(const std::vector<glm::vec3>&& positions, const std::vector<glm::vec3>&& normals, const std::vector<glm::vec3>&& colors, uint32_t id, const std::vector<unsigned>&& indecies);

	virtual void Render() const override;

private:

	std::unique_ptr<VertexArrayColoredTriangle> m_VertexArray;

	std::vector<glm::vec3> m_Positions, m_Normals, m_Colors;

	std::vector<unsigned> m_Indecies;

	uint32_t m_ID;

};