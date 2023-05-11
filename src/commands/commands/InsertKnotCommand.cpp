#pragma once

#include "InsertKnotCommand.h"

#include "../../scene/Scene.h"
#include "../TextParser.h"
#include "../../geometry/NURBS.h"

void InsertKnotCommand::TextInput(const std::string& input)
{
	float f;
	if (TextParser::ParseFloat(input, f))
		for (auto& [k, v] : Scene::GetContents())
			if (v->IsSelected()) {
				NURBS* curve = dynamic_cast<NURBS*>(v.get());
				if (curve) curve->InsertKnot(f);
			}
	m_Finished = true;
}

void InsertKnotCommand::ClickInput(int x, int y, int mods)
{
	Scene::HandleClick(x, y, mods);
}

std::string InsertKnotCommand::GetInstructions() const
{
	bool found = false;
	for (auto& [k, v] : Scene::GetContents()) {
		if (v->IsSelected()) {
			if (!dynamic_cast<NURBS*>(v.get())) {
				Scene::UnSelect(v->GetID());
			}
			else {
				found = true;
			}
		}
	}
	if (found) return "Enter knot for insertion: ";
	else return "Select curve for knot insertion.";
}


