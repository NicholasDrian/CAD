#pragma once 

#include "Scene.h"

#include "../Window.h"

Camera* Scene::m_Camera = nullptr;

void Scene::Init()
{
	m_Camera = new Camera(glm::vec3{ 0.0f, -50.0f, 25.0f }, glm::vec3{ 0.0f, 0.0f, 25.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, 45.0f);
}

void Scene::Destroy()
{
	delete m_Camera;
}
