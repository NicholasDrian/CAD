#pragma once

#include "PolyLineCommand.h"
#include "../../Window.h"

#include "glm/glm.hpp"


void PolyLineCommand::TextInput(const std::string& input)
{
	if (input == "") {
		if (m_PolyLine) {
			if (m_PolyLine->GetNumControlPoints() > 2) {
				m_PolyLine->m_Selectable = true;
				m_PolyLine->RemoveLast();
				Scene::AddToScene(std::move(m_PolyLine));
			}
		}
		m_Finished = true;
	}
	else {
		//todo
	}
}

void PolyLineCommand::ClickInput(int x, int y)
{
	if (Window::IsWithinLocal(x, y)) {
		glm::vec3 intersection;
		if (Scene::IntersectScene(x, y, intersection)) {
			if (m_PolyLine) {
				m_PolyLine->UpdateLast(intersection);
				m_PolyLine->AddPoint(intersection);
			}
			else {
				m_PolyLine = std::make_unique<PolyLine>(std::vector{ intersection, intersection });
				m_PolyLine->m_Selectable = false;
			}
		}
	}
}

void PolyLineCommand::Tick()
{
	if (m_PolyLine) {
		auto [x, y] = Window::GetCursorPosition();
		if (Window::IsWithinLocal(x, y)) {
			glm::vec3 intersection;
			if (Scene::IntersectScene((int)x, (int)y, intersection)) {
				m_PolyLine->UpdateLast(intersection);
			}
		}
	}
}

void PolyLineCommand::Escape()
{
	if (m_PolyLine) {
		Scene::Delete(m_PolyLine->GetID());
	}
	m_Finished = true;
}

void PolyLineCommand::Render() const
{
	if (m_PolyLine) m_PolyLine->Render();
}
