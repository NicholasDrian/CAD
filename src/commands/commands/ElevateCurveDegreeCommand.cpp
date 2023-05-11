#pragma once

#include "ElevateCurveDegreeCommand.h"

#include "../../scene/Scene.h"
#include "../../geometry/NURBS.h"
#include "../TextParser.h"

ElevateCurveDegreeCommand::ElevateCurveDegreeCommand()
{
	Scene::HideTransformWidget();
}

ElevateCurveDegreeCommand::~ElevateCurveDegreeCommand()
{
	Scene::ClearSelection();
	Scene::ShowTransformWidget();
}

void ElevateCurveDegreeCommand::TextInput(const std::string& input)
{
	unsigned newDegree;
	if (TextParser::ParseUnsignedInt(input, newDegree))
		for (auto& [k, v] : Scene::GetContents())
			if (v->IsSelected()) {
				NURBS* curve = dynamic_cast<NURBS*>(v.get());
				if (curve && curve->GetDegree() < newDegree)
					curve->ElevateDegree(newDegree - curve->GetDegree());
			}
	m_Finished = true;
}

void ElevateCurveDegreeCommand::ClickInput(int x, int y, int mods)
{
	Scene::HandleClick(x, y, mods);
}

std::string ElevateCurveDegreeCommand::GetInstructions() const
{
	bool found = false;
	for (auto&[k, v] : Scene::GetContents()) {
		if (v->IsSelected()) {
			if (!dynamic_cast<NURBS*>(v.get())) {
				Scene::UnSelect(v->GetID());
			}
			else {
				found = true;
			}
		}
	}
	if (found) {
		return "Enter desired degree: ";
	}
	else {
		return "Select curve to elevate.";
	}
}
