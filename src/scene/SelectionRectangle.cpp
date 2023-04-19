#pragma once

#include "SelectionRectangle.h"
#include "../Window.h"
#include "../render/Renderer.h"

#include <iostream>

SelectionRectangle::SelectionRectangle(int left, int top, int right, int bottom)
	: m_Left(left), m_Top(top), m_Right(right), m_Bottom(bottom)
{
	UpdateVertexArray();
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

	float l = (float)m_Left * 2.0f / sizex - 0.5;
	float r = (float)m_Right * 2.0f / sizex - 0.5;
	float t = (float)m_Top * 2.0f / sizey - 0.5;
	float b = (float)m_Bottom * 2.0f / sizey - 0.5;

	std::cout << l << ' ' << r << ' ' << t << ' ' << b << std::endl;

	std::vector<glm::vec3> positions = {
		glm::vec3{l, t, 0},
		glm::vec3{r, t, 0},
		glm::vec3{r, b, 0},
		glm::vec3{l, b, 0}
	};

	m_VertexArray = std::make_unique<VertexArrayScreenSpaceTriangles>(positions, std::vector<uint32_t>{0, 1, 2, 2, 3, 0}, glm::vec4{ 1.0f,1.0f,1.0f,1.0f });
}
