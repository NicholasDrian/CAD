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
	std::vector positions{ glm::vec3{ 0.0f, 0.0f, 5.0f }, glm::vec3{ 20.0f, 0.0f, 5.0f }, glm::vec3{20,20,5}, glm::vec3{ 0.0f, 20.0f, 5.0f } };
	std::vector normals { glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f } };
	glm::vec4 color{ 1.0f, 0.0f, 0.3f , 1.0f};
	std::vector indices{ 0U, 1U, 2U, 2U, 3U, 0U };
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
	if (m_SelectionRectangle) m_SelectionRectangle->Render();
}

void Scene::DrawGUI()
{
	if (m_TransformWidget) m_TransformWidget->Draw();
	ShaderManager::UpdateGlobalUniforms();
}


glm::mat4 Scene::GetSelectionTransform()
{
	if (m_TransformWidget) return m_TransformWidget->GetDelta();
	return glm::mat4(1.0);
}

void Scene::SelectionTransformUpdated()
{
	for (uint32_t id : m_Selected) m_Contents[id]->SelectionTransformUpdated();
	for (uint32_t id : m_SubSelected) m_Contents[id]->SelectionTransformUpdated();
}

void Scene::AddToScene(std::unique_ptr<Renderable> obj)
{
	m_Contents[obj->GetID()] = std::move(obj);
}

void Scene::UpdateSelectionRectangle(int left, int top, int right, int bottom)
{
	if (left != right && top != bottom) {
		if (!m_SelectionRectangle) m_SelectionRectangle = std::make_unique<SelectionRectangle>(left, top, right, bottom);
		else m_SelectionRectangle->Update(left, top, right, bottom);
	}
}

void Scene::ApplySelectionRectangle(bool subSelection, bool inclusive)
{
	if (m_SelectionRectangle) {
		//todo
		m_SelectionRectangle.reset();
	}
}

void Scene::Destroy()
{
	m_Contents.clear();
	m_ConstructionPlane.reset();
	m_Camera.reset();
	m_SelectionRectangle.reset();
}

void Scene::ClearSelection()
{
	for (uint32_t s : m_Selected) m_Contents[s]->UnSelect();
	for (uint32_t s : m_SubSelected) m_Contents[s]->ClearSubSelection();
	m_Selected.clear();
	m_SubSelected.clear();
}


AxisAlignedBoundingBox Scene::GetSelectedBoundingBox()
{
	AxisAlignedBoundingBox bb;
	for (uint32_t i : m_Selected) bb += m_Contents[i]->GetBoundingBox();
	for (uint32_t i : m_SubSelected) bb += m_Contents[i]->GetSubSelectionBoundingBox();
	return bb;
}

void Scene::HandleClick(int x, int y, int mods) 
{
	uint64_t IDs = Renderer::ReadIDAtPixel(x, y);
	bool shift = GLFW_MOD_SHIFT & mods;
	bool control = GLFW_MOD_CONTROL & mods;

	if (m_TransformWidget) {
		glm::mat4 t = GetSelectionTransform();
		for (auto& p : m_Contents) p.second->BakeSelectionTransform(t);
		m_TransformWidget.reset();
	}

	if (uint32_t ID = IDs >> 32)
	{
		uint32_t subID = IDs & 0x00000000FFFFFFFFLLU;

		if (control && shift)
		{
			if (!m_Selected.contains(ID)) {
				m_SubSelected.insert(ID);
				m_Contents[ID]->AddSubSelection(subID);
			}
		}
		else if (control)
		{
			m_Selected.erase(ID);
			m_SubSelected.erase(ID);
			m_Contents[ID]->UnSelect();
			m_Contents[ID]->RemoveSubSelection(subID);
		}
		else if (shift)
		{
			m_Selected.insert(ID);
			m_SubSelected.erase(ID);
			m_Contents[ID]->ClearSubSelection();
			m_Contents[ID]->Select();
		}
		else
		{
			ClearSelection();
			m_Selected.insert(ID);
			m_Contents[ID]->Select();
		}
	}
	else
	{
		if (!shift && !control) {
			ClearSelection();
		}
	}

	
	if (numSelected > 0) m_TransformWidget = std::make_unique<AffineTransformWidget>(GetSelectedBoundingBox());
}

// Invariant: never both selected and sub selected!
void Scene::DeleteSelection() 
{
	m_TransformWidget.reset();

	for (int i : m_Selected) m_Contents.erase(i);
	m_Selected.clear();

	// Todo: handle deletion of sub selection!

}

void Scene::Delete(unsigned id)
{
	m_Selected.erase(id);
	m_SubSelected.erase(id);
	m_Contents.erase(id);
}

bool Scene::IntersectScene(int x, int y, glm::vec3& outPoint)
{

	uint64_t IDs = Renderer::ReadIDAtPixel(x, y);
	uint32_t ID = IDs >> 32;
	Ray ray = m_Camera->GetRayAtPixel(x, y);

	if (ID == 0) {
		return ray.IntersectPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, outPoint);
	}
	else {
		Renderable* item = m_Contents[ID].get();
		uint32_t subID = IDs & 0x00000000FFFFFFFFLLU;
		outPoint = item->Intersect(ray, subID);
		return true;
	}

}

// todo fix - curve writes to id buffer even when drawn last???????? 