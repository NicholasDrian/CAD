#pragma once

#include "PolyLineCommand.h"
#include "../../Window.h"

#include "glm/glm.hpp"


void PolyLineCommand::TextInput(const std::string& input)
{
	if (input == "") {
		if (m_PolyLine) {
			if (m_PolyLine->GetNumControlPoints() < 3) {
				Scene::Delete(m_PolyLine->GetID());
			}
			else {
				m_PolyLine->m_Selectable = true;
				m_PolyLine->RemoveLast();
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
	glm::vec3 intersection;
	if (Scene::IntersectScene(x, y, intersection)) {
		if (m_PolyLine) {
			m_PolyLine->UpdateLast(intersection);
			m_PolyLine->AddPoint(intersection);
		}
		else {
			m_PolyLine = new PolyLine({ intersection, intersection });
			m_PolyLine->m_Selectable = false;
			Scene::AddToScene(std::unique_ptr<PolyLine>(m_PolyLine));
		}
	}
}

void PolyLineCommand::Tick()
{
	if (m_PolyLine) {
		double x, y;
		Window::GetCursorPosition(x, y);
		glm::vec3 intersection;
		if (Scene::IntersectScene((int)x, (int)y, intersection)) {
			m_PolyLine->UpdateLast(intersection);
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
