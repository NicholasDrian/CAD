#pragma once

#include "PointsOnCommand.h"
#include "../CommandManager.h"
#include "../../scene/Scene.h"

#include "imgui.h"

CommandPointsOn::CommandPointsOn()
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

void CommandPointsOn::TextInput(const std::string& input)
{
	if (input == "ALL") {
		auto& contents = Scene::GetContents();
		for (auto& e : contents) e.second->PointsOn();
	}
	else if (input == "") {
		auto& contents = Scene::GetContents();
		for (auto& e : contents) if (e.second->IsSelected()) e.second->PointsOn();
	}
	m_Finished = true;
	Scene::ClearSelection();
	Scene::ShowTransformWidget();
}

void CommandPointsOn::ClickInput(int x, int y, int mods)
{
	Scene::HandleClick(x, y, mods);
}

void CommandPointsOn::DrawButtons()
{
	if (ImGui::Button("ALL")) {
		TextInput("ALL");
	}
}