#pragma once 

#include "Scene.h"

#include "ConstructionPlane.h"
#include "../Window.h"
#include "../geometry/Renderable.h"
#include "../geometry/Mesh.h"
#include "../geometry/NURBS.h"
#include "../render/Renderer.h"
#include "glm/gtc/constants.hpp"
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"

#include <iostream>

void Scene::Init()
{

	//test scene
	std::vector positions { glm::vec3{ 0.0f, 0.0f, 5.0f }, glm::vec3{ 20.0f, 0.0f, 5.0f }, glm::vec3{ 0.0f, 20.0f, 5.0f } };
	std::vector normals { glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 20.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 20.0f, 0.0f } };
	glm::vec3 color{ 1.0f, 0.0f, 0.3f };
	std::vector indices{ 0U, 1U, 2U };
	std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(positions, normals, color, indices);
	AddToScene(std::move(mesh));

	std::vector<glm::vec3> points = { { 10.0f, 0.0f, 0.0f }, { 10.0f, 10.0f, 0.0f }, { 0.0f, 20.0f, 0.0f }, {100.0, -20.0, 0.0}, {100.0, 100.0, 0.0} };


	std::unique_ptr<NURBS> curve = std::make_unique<NURBS>(points, color);
	AddToScene(std::move(curve));


	m_ConstructionPlane = std::make_unique<ConstructionPlane>(10, 10, 1);
	m_Camera = std::make_unique<Camera>(glm::vec3{ 0.0f, -50.0f, 50.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::pi<float>() / 3.0f);
}

void Scene::Render() {
	m_ConstructionPlane->Render();
	for (const auto& entry : m_Contents) {
		entry.second->Render();
	}
}

void Scene::AddToScene(std::unique_ptr<Renderable> obj)
{
	m_Contents[obj->GetID()] = std::move(obj);
}

void Scene::Destroy()
{
	m_Contents.clear();
	m_ConstructionPlane.reset();
	m_Camera.reset();
}


// Shift to select multiple, Control to unselect indevidualy
void Scene::HandleClick(int x, int y, int button, int mods) 
{
	int id = Renderer::ReadIDAtPixel(x, y);
	if (mods != GLFW_MOD_SHIFT && mods != GLFW_MOD_CONTROL) {
		for (unsigned oldSelection : m_Selected) {
			m_Contents[oldSelection]->m_Selected = false;
		}
		m_Selected.clear();
	}

	if (id != 0) {

		if (mods == GLFW_MOD_CONTROL) 
		{
			if (m_Selected.contains(id)) {
				m_Contents[id]->m_Selected = false;
				m_Selected.erase(id);
			}
		}
		else 
		{
			m_Selected.insert(id);
			m_Contents[id]->m_Selected = true;
		}
	}
}

void Scene::DeleteSelection() 
{
	for (int i : m_Selected) 
	{
		m_Contents.erase(i);
	}
	m_Selected.clear();
}

void Scene::Delete(unsigned id)
{
	m_Contents.erase(id);
}

bool Scene::IntersectScene(int x, int y, glm::vec3& outPoint)
{
	int id = Renderer::ReadIDAtPixel(x, y);
	Ray ray = m_Camera->GetRayAtPixel(x, y);

	if (id == 0) {
		ray.IntersectPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, outPoint);
		return ray.IntersectPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, outPoint);
	}
	else {
		outPoint = ray.At(Renderer::ReadDistanceAtPixel(x, y));
		return true;
	}
}