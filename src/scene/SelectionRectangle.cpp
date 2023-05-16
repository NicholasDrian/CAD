#pragma once

#include "SelectionRectangle.h"
#include "../Window.h"
#include "../render/Renderer.h"
#include "Scene.h"

#include <iostream>

SelectionRectangle::SelectionRectangle(int left, int top, int right, int bottom)
	: m_Left(left), m_Top(top), m_Right(right), m_Bottom(bottom)
{
	UpdateVertexArray();
}

Frustum SelectionRectangle::GetFrustum() const
{
	return Frustum(
		Scene::GetCamera()->GetPosition(),
		Scene::GetCamera()->GetRayAtPixel(m_Left, m_Top).GetDirection(),
		Scene::GetCamera()->GetRayAtPixel(m_Right, m_Top).GetDirection(),
		Scene::GetCamera()->GetRayAtPixel(m_Left, m_Bottom).GetDirection(),
		Scene::GetCamera()->GetRayAtPixel(m_Right, m_Bottom).GetDirection()
	);
}

void SelectionRectangle::Update(int left, int top, int right, int bottom)
{
	m_Left = left, m_Top = top, m_Right = right, m_Bottom = bottom;
	UpdateVertexArray();
}

void SelectionRectangle::Render() const
{
	m_VertexArray->Render();
}

void SelectionRectangle::UpdateVertexArray()
{
	auto [sizex, sizey] = Window::GetSize();

	float l = (float)m_Left * 2.0f / sizex - 1.0f;
	float r = (float)m_Right * 2.0f / sizex - 1.0f;
	float t = (float)m_Top * -2.0f / sizey + 1.0f;
	float b = (float)m_Bottom * -2.0f / sizey + 1.0f;

	std::vector<glm::vec3> positions = {
		glm::vec3{l, t, 0},
		glm::vec3{r, t, 0},
		glm::vec3{r, b, 0},
		glm::vec3{l, b, 0}
	};

	m_VertexArray = std::make_unique<VertexArrayScreenSpaceTriangles>(
		positions, 
		std::vector<uint32_t>{0, 1, 2, 2, 3, 0}, 
		glm::vec4{ 0.0f, 0.0f, 0.0f, 0.3f }
	);
}