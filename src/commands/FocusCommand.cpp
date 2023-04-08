#pragma once

#include "FocusCommand.h"
#include "../scene/Camera.h"
#include "../scene/Scene.h"
#include "CommandManager.h"
#include "TextParser.h"
#include "glm/gtx/intersect.hpp"
#include "../render/Renderer.h"

#include <iostream>

void FocusCommand::TextInput(const std::string& input) {
	glm::vec3 focalPoint;
	if (TextParser::ParseVector3F(input, focalPoint)) {
		// TODO
	}
	m_Finished = true;
}


void FocusCommand::ClickInput(int x, int y) 
{
	glm::vec3 intersection;
	if (Scene::IntersectScene(x, y, intersection)) {
		Scene::GetCamera()->RepositionFocal(intersection);
	}
	m_Finished = true;
}