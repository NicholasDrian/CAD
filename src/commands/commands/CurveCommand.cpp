#pragma once

#include "CurveCommand.h"
#include "../TextParser.h"
#include "../../Window.h"

#include "imgui.h"

void CurveCommand::TextInput(const std::string& input)
{
	if (m_Mode == CurveCommandMode::AddPoint) {
		if (input == "") {
			if (m_Curve && m_Curve->GetNumControlPoints() + m_NeedsExtraPoint > 2) {
				m_Curve->m_Selectable = true;
				if (!m_NeedsExtraPoint) m_Curve->RemoveLastPoint();
				Scene::AddToScene(std::move(m_Curve));
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
			if (m_Curve) m_Curve->ChangeDegree(m_Degree);
		}
		m_Mode = CurveCommandMode::AddPoint;
	}
}

void CurveCommand::ClickInput(int x, int y, int mods)
{
	if (Window::IsWithinLocal(x, y) && m_Mode == CurveCommandMode::AddPoint) {
		glm::vec3 intersection;
		if (Scene::IntersectScene(x, y, intersection)) {
			if (m_Curve)
			{
				if (m_NeedsExtraPoint) m_Curve->AddControlPoint(intersection, m_Curve->GetDegree() != m_Degree);
				m_NeedsExtraPoint = true;
			}
			else
			{
				std::vector<glm::vec3> points = { intersection, intersection };
				m_Curve = std::make_unique<NURBS>(points, glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });
				m_Curve->m_Selectable = false;
			}
		}
	}
}

void CurveCommand::Tick()
{
	if (m_Mode == CurveCommandMode::AddPoint && m_Curve) {
		auto [x, y] = Window::GetCursorPosition();
		if (Window::IsWithinLocal(x, y) && (x != m_PrevX || y != m_PrevY)) {
			glm::vec3 intersection;
			if (Scene::IntersectScene(x, y, intersection)) {
				if (m_NeedsExtraPoint) {
					m_Curve->AddControlPoint(intersection, m_Curve->GetDegree() != m_Degree);
					m_NeedsExtraPoint = false;
				}
				else m_Curve->UpdateLastPoint(intersection);
			}
			m_PrevX = x;
			m_PrevY = y;
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

void CurveCommand::Render() const
{
	if (m_Curve) m_Curve->Render();
}