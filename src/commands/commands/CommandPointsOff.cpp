#pragma once

#include "CommandPointsOff.h"
#include "../CommandManager.h"
#include "../../scene/Scene.h"

#include "imgui.h"

CommandPointsOff::CommandPointsOff()
{
	bool selected = false;
	auto& contents = Scene::GetContents();
	for (auto& e : contents) if (e.second->IsSelected())
	{
		e.second->PointsOn();
		selected = true;
	}
	if (selected) m_Finished = true;
	else Scene::HideTransformWidget();
}

void CommandPointsOff::TextInput(const std::string& input)
{
	if (input == "ALL") {
		auto& contents = Scene::GetContents();
		for (auto& e : contents) e.second->PointsOff();
	}
	else if (input == "") {
		auto& contents = Scene::GetContents();
		for (auto& e : contents) if (e.second->IsSelected()) e.second->PointsOff();
	}
	m_Finished = true;
	Scene::ClearSelection();
	Scene::ShowTransformWidget();
}

void CommandPointsOff::ClickInput(int x, int y, int mods)
{
	Scene::HandleClick(x, y, mods);
}

void CommandPointsOff::DrawButtons()
{
	if (ImGui::Button("ALL")) {
		TextInput("ALL");
	}
}
