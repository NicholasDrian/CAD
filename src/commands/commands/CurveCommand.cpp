#pragma once

#include "CurveCommand.h"
#include "../TextParser.h"
#include "../../Window.h"

#include "imgui.h"

void CurveCommand::TextInput(const std::string& input)
{
	if (m_Mode == CurveCommandMode::AddPoint) {
		if (input == "") {
			if (m_Curve->GetNumControlPoints() < 3) {
				Scene::Delete(m_Curve->GetID());
			}
			else {
				m_Curve->m_Selectable = true;
				if (!m_NeedsExtraPoint) m_Curve->RemoveLastPoint();
			}
			m_Finished = true;
		}
		else {
			//todo
		}
	}
	else if (m_Mode == CurveCommandMode::ChangeDegree) {
		unsigned newDegree;
		if (TextParser::ParseUnsignedInt(input, newDegree)) {
			m_Degree = newDegree;
			std::cout << m_Degree << std::endl;
			if (m_Curve) m_Curve->ChangeDegree(m_Degree);
		}
		m_Mode = CurveCommandMode::AddPoint;
	}
}

void CurveCommand::ClickInput(int x, int y)
{
	if (m_Mode == CurveCommandMode::AddPoint) {
		glm::vec3 intersection;
		if (Scene::IntersectScene(x, y, intersection)) {
			if (m_Curve)
			{
				m_NeedsExtraPoint = true;
			}
			else
			{
				glm::vec3 color(0.0f, 0.0f, 0.0f);
				std::vector<glm::vec3> points = { intersection, intersection };
				m_Curve = new NURBS(points, color);
				m_Curve->m_Selectable = false;
				Scene::AddToScene(std::unique_ptr<NURBS>(m_Curve));
			}
		}
	}
}

void CurveCommand::Tick()
{
	if (m_Mode == CurveCommandMode::AddPoint) {
		if (m_Curve) {
			double x, y;
			Window::GetCursorPosition(x, y);
			if ((int)x != m_PrevX || (int)y != m_PrevY) {
				glm::vec3 intersection;
				if (Scene::IntersectScene((int)x, (int)y, intersection)) {
					if (m_NeedsExtraPoint) {
						m_Curve->AddControlPoint(intersection, m_Curve->GetDegree() != m_Degree);
						m_NeedsExtraPoint = false;
					}
					else m_Curve->UpdateLastPoint(intersection);
				}
				m_PrevX = (int)x;
				m_PrevY = (int)y;
			}
		}
	}
}

void CurveCommand::Escape()
{
	if (m_Curve) Scene::Delete(m_Curve->GetID());
	m_Finished = true;
}

void CurveCommand::DrawButtons()
{
	if (m_Mode != CurveCommandMode::ChangeDegree) {
		std::string degreeButtonLabel = "Degree = " + std::to_string(m_Degree);
		if (ImGui::Button(degreeButtonLabel.c_str())) {
			m_Mode = CurveCommandMode::ChangeDegree;
		}
	}
}

std::string CurveCommand::GetInstructions() const
{
	switch (m_Mode) {
	case CurveCommandMode::ChangeDegree:
		return "Enter desired degree:";
	case CurveCommandMode::AddPoint:
		if (m_Curve) return "Click or type next point:";
		return "Click or type start point:";
	default:
		return "INTERNAL_ERROR";
	}
}
