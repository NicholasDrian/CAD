#pragma once 

#include "Scene.h"

#include "../Window.h"
#include "../geometry/Renderable.h"
#include "../geometry/Triangle.h"

#include "glm/glm.hpp"

Camera* Scene::m_Camera = nullptr;

std::vector<std::unique_ptr<Renderable>> Scene::m_Contents;

void Scene::Init()
{
	m_Camera = new Camera(glm::vec3{ 0.0f, -50.0f, 25.0f }, glm::vec3{ 0.0f, 0.0f, 25.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, 45.0f);

	//test scene
	m_Contents.push_back(std::make_unique<Triangle>(
		glm::vec3{ 0.0f, 0.0f, 0.0f }, 
		glm::vec3{ 50.0f, 0.5f, 0.0f }, 
		glm::vec3{ 0.5f, 50.0f, 0.0f }
	));
}

void Scene::Render() {
	for (const std::unique_ptr<Renderable>& item : m_Contents) {
		item->Render();
	}
}

void Scene::Destroy()
{
	delete m_Camera;
}
