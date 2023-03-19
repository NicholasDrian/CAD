#pragma once 

#include "Scene.h"

#include "../Window.h"
#include "../geometry/Renderable.h"
#include "../geometry/Mesh.h"

#include "glm/glm.hpp"

void Scene::Init()
{

	//test scene
	std::vector positions { glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 50.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 50.0f, 0.0f } };
	std::vector normals { glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 50.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 50.0f, 0.0f } };
	std::vector colors { glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f } };
	unsigned id = 69U;
	std::vector indecies{ 0U, 1U, 2U };
	m_Contents.push_back(std::make_unique<Mesh>(positions, normals, colors, id, indecies));

	m_Camera = new Camera(glm::vec3{ 0.0f, -50.0f, 25.0f }, glm::vec3{ 0.0f, 0.0f, 25.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, 45.0f);
}

void Scene::Render() {
	for (const std::unique_ptr<Renderable>& item : m_Contents) {
		item->Render();
	}
}

void Scene::Destroy()
{
	delete m_Camera;
	m_Contents.clear();
}
