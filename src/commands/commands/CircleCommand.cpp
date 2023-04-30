#pragma once

#include "CircleCommand.h"
#include "../../Window.h"
#include "../TextParser.h"

#include "../../debug/Print.h"

#include "imgui.h"

// TODO: Add failure checks. For example, three points cannot be co-linear.

void CircleCommand::TextInput(const std::string& input)
{
	if (input == "") {
		m_Finished = true;
	}
	if (m_Mode == CircleCommandMode::None) {
		unsigned newMode;
		if (TextParser::ParseUnsignedInt(input, newMode)) {
			switch (newMode) {
			case 1:
				m_Mode = CircleCommandMode::NormalCenterRadious;
				break;
			case 2:
				m_Mode = CircleCommandMode::CenterPointPoint;
				break;
			case 3:
				m_Mode = CircleCommandMode::PointPointPoint;
				break;
			}
		}
	}
	// TODO
}

void CircleCommand::ClickInput(int x, int y, int mods)
{
	if (Window::IsWithinLocal(x, y)) {
		if (m_Mode == CircleCommandMode::NormalCenterRadious) {
			switch (m_Phase) {
			case 0:
				Scene::IntersectScene(x, y, a);
				m_Phase++;
				break;
			case 1:
				Scene::IntersectScene(x, y, b);
				a = b - a;
				m_Phase++;
				break;
			case 2:
				Scene::IntersectScene(x, y, b);
				m_Phase++;
				break;
			default:
				Scene::IntersectScene(x, y, c);
				m_Circle = std::make_unique<Circle>(a, b, glm::distance(b, c));
				Scene::AddToScene(std::move(m_Circle));
				m_Finished = true;
			}
		}
		else if (m_Mode == CircleCommandMode::CenterPointPoint) {
			switch (m_Phase) {
			case 0:
				Scene::IntersectScene(x, y, a);
				m_Phase++;
				break;
			case 1:
				Scene::IntersectScene(x, y, b);
				m_Phase++;
				break;
			default:
				Scene::IntersectScene(x, y, c);
				glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));
				m_Circle = std::make_unique<Circle>(normal, a, glm::distance(a, c));
				Scene::AddToScene(std::move(m_Circle));
				m_Finished = true;
				break;
			}
		}
		else if (m_Mode == CircleCommandMode::PointPointPoint) {
			switch (m_Phase) {
			case 0:
				Scene::IntersectScene(x, y, a);
				m_Phase++;
				break;
			case 1:
				Scene::IntersectScene(x, y, b);
				m_Phase++;
				break;
			default:
				Scene::IntersectScene(x, y, c);
				m_Circle = std::make_unique<Circle>(a, b, c);
				Scene::AddToScene(std::move(m_Circle));
				m_Finished = true;
				break;
			}
		}
	}
}

std::string CircleCommand::GetInstructions() const
{
	if (m_Mode == CircleCommandMode::None) {
		return "CLICK OR TYPE MODE: ";
	}
	else if (m_Mode == CircleCommandMode::NormalCenterRadious) {
		switch (m_Phase) {
		case 0: return "CLICK START OF NORMAL AXIS OR TYPE NORMAL VECTOR: ";
		case 1: return "CLICK END OF NORMAL AXIS: ";
		case 2: return "CLICK OR TYPE CENTER POINT: ";
		default: return "TYPE RADIOUS OR CLICK PERIMITER POINT: ";
		}
	}
	else if (m_Mode == CircleCommandMode::CenterPointPoint) {
		switch (m_Phase) {
		case 0: return "CLICK OR TYPE CENTER POINT: ";
		default: return "CLICK OR TYPE PERIMITER POINT: ";
		}
	}
	else {
		return "CLICK OR TYPE PERIMITER POINT: ";
	}
}

void CircleCommand::Tick()
{
	auto [x, y] = Window::GetCursorPosition();
	if (Window::IsWithinLocal(x, y)) {
		Scene::IntersectScene(x, y, c);
		if (m_Mode == CircleCommandMode::NormalCenterRadious && m_Phase == 3) {
			m_Circle = std::make_unique<Circle>(a, b, glm::distance(b, c));
			m_Circle->m_Selectable = false;
		}
		else if (m_Mode == CircleCommandMode::CenterPointPoint && m_Phase == 2) {
			glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));
			m_Circle = std::make_unique<Circle>(normal, a, glm::distance(a, c));
			m_Circle->m_Selectable = false;
		}
		else if (m_Mode == CircleCommandMode::PointPointPoint && m_Phase == 2) {
			m_Circle = std::make_unique<Circle>(a, b, c);
			m_Circle->m_Selectable = false;
		}
	}
}

void CircleCommand::DrawButtons()
{
	if (m_Mode == CircleCommandMode::None) {
		if (ImGui::Button("1. Normal, Center, Radious")) 
		{
			m_Mode = CircleCommandMode::NormalCenterRadious;
		}
		if (ImGui::Button("2. Center, 2Points"))
		{
			m_Mode = CircleCommandMode::CenterPointPoint;
		}
		if (ImGui::Button("3. 3Points")) {
			m_Mode = CircleCommandMode::PointPointPoint;
		}
	}
}

void CircleCommand::Render() const
{
	if (m_Circle) m_Circle->Render();
}
