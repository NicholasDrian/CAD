#pragma once

#include "TextParser.h"

#include "GLFW/glfw3.h"


bool TextParser::IsTypable(int character)
{
	// to be expanded
	if (character >= GLFW_KEY_A && character <= GLFW_KEY_Z) return true;
	if (character >= GLFW_KEY_0 && character <= GLFW_KEY_9) return true;
	if (character >= GLFW_KEY_A && character <= GLFW_KEY_Z) return true;
	if (character == GLFW_KEY_SPACE) return true;
	if (character == GLFW_KEY_COMMA) return true;
	if (character == GLFW_KEY_MINUS) return true;
	if (character == GLFW_KEY_PERIOD) return true;
	return false;
}

bool TextParser::ParseVector3F(const std::string& text, glm::vec3& outVector) 
{
	return false;
}