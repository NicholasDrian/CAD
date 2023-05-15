#pragma once

#include "SettingsGUI.h"

#include "imgui.h"

void SettingsGUI::Render()
{
	if (ImGui::CollapsingHeader("Snaps")) {
		ImGui::Checkbox("Curve", &m_SnapCurve);
		ImGui::Checkbox("Point", &m_SnapPoint);
		ImGui::Checkbox("Grid", &m_SnapGrid);
	}
	if (ImGui::CollapsingHeader("Display")) {

	}
}
