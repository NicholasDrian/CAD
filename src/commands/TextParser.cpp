#pragma once

#include "TextParser.h"

#include "GLFW/glfw3.h"

#include <iostream>


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

bool TextParser::ParseUnsignedInt(const std::string& text, unsigned& outUnsignedInt)
{
	int i = 0;
	return NextUnsignedInt(text, i, outUnsignedInt);
}

bool TextParser::NextUnsignedInt(const std::string& text, int& i, unsigned& outUnsignedInt)
{	
	outUnsignedInt = 0;
	bool res = false;
	while (i < text.size() && !IsDigit(text[i]))i++;
	while (i < text.size() && IsDigit(text[i])) {
		res = true;
		outUnsignedInt *= 10;
		outUnsignedInt += ToDigit(text[i++]);
	}
	while (i < text.size() && text[i] == GLFW_KEY_PERIOD || IsDigit(text[i])) i++;
	return res;
}

bool TextParser::NextFloat(const std::string& text, int& i, float& outFloat)
{
	// todo
	return false;
}

bool TextParser::IsDigit(char c)
{
	return c >= '0' && c <= '9';
}

int TextParser::ToDigit(char c)
{
	return c - '0';
}
