#pragma once

#include "Mesh.h"
#include "../render/Renderer.h"

Mesh::Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<glm::vec3>& colors, uint32_t id, const std::vector<unsigned>& indecies)
	: m_Positions(positions), m_Normals(normals), m_Colors(colors), m_ID(id), m_Indecies(indecies), m_VertexArray(std::make_unique<VertexArrayColoredTriangle>(positions, normals, colors, id, indecies))
{

}

void Mesh::Render() const {
	Renderer::Render(m_VertexArray.get());
}