#pragma once

#include "FocusCommand.h"
#include "../scene/Camera.h"
#include "CommandManager.h"
#include "TextParser.h"

#include "../render/Renderer.h"

#include <iostream>

void FocusCommand::TextInput(const std::string& input) {
	glm::vec3 focalPoint;
	if (TextParser::ParseVector3F(input, focalPoint)) {
		// change focus
	}
	CommandManager::TerminateActiveCommand();
}


void FocusCommand::ClickInput(int x, int y) 
{
	int id = Renderer::ReadIDAtPixel(x, y);
	if (id == 0) {
		// intersect with ground plane or do nothing
	}
	float distance = Renderer::ReadDistanceAtPixel(x, y);



	CommandManager::TerminateActiveCommand();
}