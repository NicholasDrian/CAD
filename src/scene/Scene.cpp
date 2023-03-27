#pragma once 

#include "Scene.h"

#include "ConstructionPlane.h"
#include "../Window.h"
#include "../geometry/Renderable.h"
#include "../geometry/Mesh.h"

#include "glm/glm.hpp"

void Scene::Init()
{

	//test scene
	std::vector positions { glm::vec3{ 0.0f, 0.0f, 5.0f }, glm::vec3{ 20.0f, 0.0f, 5.0f }, glm::vec3{ 0.0f, 20.0f, 5.0f } };
	std::vector normals { glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 20.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 20.0f, 0.0f } };
	std::vector colors { glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f } };
	std::vector indecies{ 0U, 1U, 2U };
	unsigned meshID = m_IDGenerator++;
	m_Contents[meshID] = std::make_unique<Mesh>(positions, normals, colors, meshID, indecies);
	m_ConstructionPlane = std::make_unique<ConstructionPlane>(10, 10, 1);
	m_Camera = std::make_unique<Camera>(glm::vec3{ 0.0f, -50.0f, 25.0f }, glm::vec3{ 0.0f, 0.0f, 25.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, 45.0f);
}

void Scene::Render() {
	m_ConstructionPlane->Render();
	for (const auto& entry : m_Contents) {
		entry.second->Render();
	}
}

void Scene::Destroy()
{
	m_Contents.clear();
	m_ConstructionPlane.reset();
	m_Camera.reset();
}
