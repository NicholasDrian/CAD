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
	uint64_t IDs = Renderer::ReadIDAtPixel(x, y);
	if (uint32_t ID = IDs >> 32)
	{
		uint32_t subID = IDs & 0x00000000FFFFFFFFLLU;
		bool shift = GLFW_MOD_SHIFT & mods;
		bool control = GLFW_MOD_CONTROL & mods;

		if (control && shift)
		{
			m_SubSelected.insert(IDs);
			m_Contents[ID]->AddSubSelection(subID);
		}
		else if (control)
		{
			if (m_Selected.contains(ID))
			{
				m_Selected.erase(ID);
				m_Contents[ID]->m_Selected = false;
			}
			if (m_SubSelected.contains(IDs))
			{
				m_SubSelected.erase(IDs);
				m_Contents[ID]->RemoveSubSelection(subID);
			}
		}
		else if (shift)
		{
			m_Selected.insert(ID);
			m_Contents[ID]->m_Selected = true;
		}
		else
		{
			for (uint32_t selection : m_Selected) m_Contents[selection]->m_Selected = false;
			for (uint64_t subSelection : m_SubSelected) m_Contents[subSelection >> 32]->ClearSubSelection();
			m_Selected.insert(ID);
			m_Contents[ID]->m_Selected = true;
		}
	}
	else
	{
		for (uint32_t selection : m_Selected) m_Contents[selection]->m_Selected = false;
		for (uint64_t subSelection : m_SubSelected) m_Contents[subSelection >> 32]->ClearSubSelection();
	}
}

void Scene::DeleteSelection() 
{
	// fix!
	for (int i : m_Selected) 
	{
		m_Contents.erase(i);
	}
	m_Selected.clear();
}

void Scene::Delete(unsigned id)
{
	// fix!
	m_Contents.erase(id);
}

bool Scene::IntersectScene(int x, int y, glm::vec3& outPoint)
{
	uint64_t IDs = Renderer::ReadIDAtPixel(x, y);
	uint32_t ID = IDs >> 32;
	Ray ray = m_Camera->GetRayAtPixel(x, y);

	if (ID == 0) {
		ray.IntersectPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, outPoint);
		return ray.IntersectPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, outPoint);
	}
	else {
		//fix!
		outPoint = ray.At(Renderer::ReadDistanceAtPixel(x, y));
		return true;
	}
}