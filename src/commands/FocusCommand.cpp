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
	CommandManager::TerminateActiveCommand();
}


void FocusCommand::ClickInput(int x, int y) 
{
	int id = Renderer::ReadIDAtPixel(x, y);
	Ray ray = Scene::GetCamera()->GetRayAtPixel(x, y);
	ray.Print();
	if (id == 0) {
		glm::vec3 intersection;
		bool intersected = ray.IntersectPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, intersection);
		if (intersected) {
			Scene::GetCamera()->RepositionFocal(intersection);
		}
	}
	else {
		float distance = Renderer::ReadDistanceAtPixel(x, y);
		glm::vec3 intersection = ray.At(distance);
		Scene::GetCamera()->RepositionFocal(intersection);
	}


	CommandManager::TerminateActiveCommand();
}