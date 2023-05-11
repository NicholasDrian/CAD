#pragma once

#include "CommandInfoGUI.h"

#include "imgui.h"

#include <algorithm>

std::vector<CommandInfoEntry> CommandInfoGUI::m_Entries = {
	{ "Circle", {
		"Construct a circle from a normal, center point, and radious",
		"Construct a circle from a center point, co-planar point, and perimiter point",
		"Construct a circle from three perimiter points"
	}},
	{ "Curve", {
		"Construct a NURBS curve of any degree based on control points"
	}},
	{ "Elevate Curve Degree", {
		"Increase the degree of a curve by n"
	}},
	{ "Focus", {
		"Moves camera based on desired focal point location"
	}},
	{ "Points Off", {
		"Hides the control points of given geometry"
	}},
	{ "Points On", {
		"Shows the control points of given geometry"
	}},
	{ "Polyline", {
		"Creates a series of line segments between control points"
	}},
};

void CommandInfoGUI::Init()
{
	std::sort(m_Entries.begin(), m_Entries.end(), [](const CommandInfoEntry& a, const CommandInfoEntry& b) {
		return a.m_Title < b.m_Title;
	});
}

void CommandInfoGUI::Render()
{
	ImGui::PushTextWrapPos();
	for (const auto& entry : m_Entries) {
		if (ImGui::CollapsingHeader(entry.m_Title.c_str())) {
			for (const auto& bullet : entry.m_BulletPoints) {
				ImGui::Bullet();
				ImGui::TextUnformatted(bullet.c_str());
			}
		}
	}
	ImGui::PopTextWrapPos();
}
