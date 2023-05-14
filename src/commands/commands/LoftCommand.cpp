#pragma once

#include "LoftCommand.h"

#include "../../scene/Scene.h"
#include "../../render/Renderer.h"
#include "../../geometry/NURBS/NURBSFactory.h"
#include "../TextParser.h"

#include "imgui.h"

LoftCommand::LoftCommand()
{
	Scene::ClearSelection();
	Scene::HideTransformWidget();
}

LoftCommand::~LoftCommand()
{
	Scene::ClearSelection();
	Scene::ShowTransformWidget();
}

void LoftCommand::TextInput(const std::string& input)
{
	unsigned val;
	if (TextParser::ParseUnsignedInt(input, val)) {
		m_Degree = val;
	} else if (input == "") {
		if (m_Curves.size()) {
			std::cout << "Lofting" << std::endl;
			Scene::AddToScene(std::make_unique<NURBSurface>(NURBSFactory::Loft(m_Curves, m_Degree)));
			std::cout << "done" << std::endl;
		}
		m_Finished = true;
	}
}

void LoftCommand::ClickInput(int x, int y, int mods)
{
	if (unsigned id = Renderer::ReadIDAtPixel(x, y) >> 32) {
		if (NURBS* curve = dynamic_cast<NURBS*>(Scene::GetContents()[id].get())) {
			m_Curves.push_back(curve);
			Scene::GetContents()[id]->Select();
		}
	}
}

std::string LoftCommand::GetInstructions() const
{
	return "Click curves to loft or enter loft degree: ";
}

void LoftCommand::Tick()
{
	for (auto& e : Scene::GetContents()) {
		if (e.second->IsSelected()) {
			if (NURBS* curve = dynamic_cast<NURBS*>(e.second.get())) {
				m_Curves.push_back(curve);
			}
			else {
				e.second->UnSelect();
			}
		}
	}
}
